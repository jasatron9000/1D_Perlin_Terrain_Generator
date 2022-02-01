#include "inc\TerrainGenerator.h"

/* TODO: Refactor the code to be more easily managed
		 e.g. use a 2D map instead of 1D and use
		 structs to deal with the different sized data*/

void PerlinNoise1D::GenerateNoise()
{
	// Initialise all the necessary values
	unsigned int _ARRAY_SIZE = m_arraySize; // Makes sure that the array size does not change
	unsigned int sampleNoiseSize = (unsigned int)pow(2U, (m_octave - 1U)) + 1U;

	float* sampledNoise = new float[sampleNoiseSize];

	for (unsigned int i = 0; i < _ARRAY_SIZE; i++) {
		m_perlinNoiseMap[i] = 0.0f;		// Initialise the perlin noise map
	}

	unsigned int n = _ARRAY_SIZE;
	float max = 0.0f, min = 999999.0f;

	// Loop through the octaves
	for (unsigned int octave = 0; octave < m_octave; octave++) {
		unsigned int idx = 0;

		// Sample the noise
		for (unsigned int s = 0; s < (m_arraySize / n); s++) {
			float funcNoise = m_noiseMap[idx * n % _ARRAY_SIZE];
			float noiseValue = (float)pow(m_persistence, octave);
			sampledNoise[s] = noiseValue * funcNoise;
			idx++;
		}

		sampledNoise[idx] = sampledNoise[0U];

		// Interpolate between points
		for (unsigned int i = 0; i < idx; i++) {
			for (unsigned int j = 0; j < n; j++) {
				float y0 = sampledNoise[i];
				float y1 = sampledNoise[i + 1U];

				m_perlinNoiseMap[((i * n) + j)] += CosineInterpolation(y0, y1, (float)j / (float)n);
			}
		}

		// Update the n value
		n = n >> 1;

	}

	// Normalise the noise map
	for (unsigned int i = 0; i < _ARRAY_SIZE; i++) {
		if (m_perlinNoiseMap[i] > max) {
			max = m_perlinNoiseMap[i];
		}

		if (m_perlinNoiseMap[i] < min) {
			min = m_perlinNoiseMap[i];
		}
	}

	for (unsigned int i = 0; i < _ARRAY_SIZE; i++) {
		std::cout << max << ", " << min << std::endl;
		float normMax = max - min;
		m_perlinNoiseMap[i] = ((m_perlinNoiseMap[i] - min) / normMax);
	}

	// Delete all declared data in the heap
	delete[] sampledNoise;
}

bool PerlinNoise1D::IsPowerOfTwo(unsigned int x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

float PerlinNoise1D::NoiseFunction(const unsigned int index, const unsigned int arraySize, const float* sampledNoise)
{
	return sampledNoise[index % arraySize];
}

float PerlinNoise1D::CosineInterpolation(const float start, const float end, const float a)
{
	float g = (1.0f - (float)cos((float)M_PI * a)) / 2.0f;
	return ((1.0f - g) * start) + (g * end);
}

PerlinNoise1D::PerlinNoise1D(const unsigned int arraySize, const unsigned int octave, const float persistence, const float maxHeight)
	:m_arraySize(arraySize), m_octave(octave), m_persistence(persistence), m_maxHeight(maxHeight)
{
	// Make checks
	// MAKE SURE THAT THE arraySize IS A POWER OF TWO
	ASSERT(IsPowerOfTwo(arraySize)); 
	for (unsigned int o = 0; o < m_octave; o++) {
		m_normaliseMax += (float)pow(m_persistence, o);

		// CHECK IF THE OCTAVE IS TOO BIG
		ASSERT(m_arraySize / (float)pow(2, o)); 
	}

	// Initialise pseudorandom generator
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_real_distribution<float> uniform(0.0f, 1.0f);

	// Generate random sequence of noise
	m_noiseMap = new float[arraySize];
	m_perlinNoiseMap = new float[arraySize];

	for (unsigned int i = 0; i < arraySize; i++) {
		m_noiseMap[i] = uniform(gen);	// Generate a random noise
	}

	// Initialise the noise
	GenerateNoise();
	
}

PerlinNoise1D::~PerlinNoise1D()
{
	delete[] m_noiseMap;
	delete[] m_perlinNoiseMap;
}

void PerlinNoise1D::ResetNoise()
{
	// Initialise pseudorandom generator
	std::random_device rd;
	std::default_random_engine gen(rd());
	std::uniform_real_distribution<float> uniform(0.0f, 1.0f);

	for (unsigned int i = 0; i < m_arraySize; i++) {
		m_noiseMap[i] = uniform(gen);	// Generate a random noise
	}

	// Initialise the noise
	GenerateNoise();
}

void PerlinNoise1D::ResetNoise(const unsigned int arraySize)
{
}


/*
*		TerrainGenerator class
* -----------------------------------------------
*  This class is used to generate a vertex data
*  for OpenGL to render.
*/
void TerrainGenerator::TerrainInit()
{
	// Generate vertices
	PerlinNoise1D noise(256, 8, 0.5, 64.0f);

	for (unsigned int z = 0; z < m_worldSize; z++) {
		for (unsigned int x = 0; x < m_worldSize; x++) {
			unsigned int idx = ((z * m_worldSize) + x);

			// Set the vertex
			float height = (noise.getNoise(x) * 0.5f) + (noise.getNoise(z) * 0.5f);
			
			m_terrainVertices[(idx * 6) + 0] = (float)x * m_scalingFactor;
			m_terrainVertices[(idx * 6) + 1] = (float)(height * 64.0f) * m_scalingFactor;
			m_terrainVertices[(idx * 6) + 2] = -1.0f * (float)z * m_scalingFactor;

			// Set the colour
			RGB colour = HeightColourPicker(height, 64.0f);
			m_terrainVertices[(idx * 6) + 3] = colour.R;
			m_terrainVertices[(idx * 6) + 4] = colour.G;
			m_terrainVertices[(idx * 6) + 5] = colour.B;
		}
	}
	std::cout << "\n________________________________" << "END" << std::endl;

	// Generate indices
	std::cout << "Generating the indices" << std::endl;
	unsigned int indicesIdx = 0;
	for (unsigned int zI = 0; zI < m_worldSize - 1; zI++) {
		for (unsigned int xI = 0; xI < m_worldSize - 1; xI++) {

			unsigned int idx = ((zI * (m_worldSize - 1)) + xI);

			// Top Triangle
			m_terrainIndices[(idx * 6) + 0] = indicesIdx;
			m_terrainIndices[(idx * 6) + 1] = indicesIdx + 1;
			m_terrainIndices[(idx * 6) + 2] = (indicesIdx + m_worldSize) + 1;

			// Bottom Triangle	
			m_terrainIndices[(idx * 6) + 3] = (indicesIdx + m_worldSize) + 1;
			m_terrainIndices[(idx * 6) + 4] = indicesIdx + m_worldSize;
			m_terrainIndices[(idx * 6) + 5] = indicesIdx;

			indicesIdx++;

		}
		indicesIdx++;
	}
}

RGB TerrainGenerator::HeightColourPicker(const float height, const float heightMax)
{
	float normHeight = height;

	if (normHeight < 0.2f) {
		return NormRGB(49, 54, 140); // WATER
	}
	else if (normHeight < 0.4f) {
		return NormRGB(230, 205, 149); // SAND
	}
	else if (normHeight < 0.6f) {
		return NormRGB(49, 140, 52); // GREEN
	}
	else if (normHeight < 0.9f){
		return NormRGB(69, 68, 63); // Rock
	}

	return NormRGB(190, 204, 204);
}

TerrainGenerator::TerrainGenerator(unsigned int worldSize, float scalingFactor, RGB colour)
	: m_worldSize(worldSize), m_scalingFactor(scalingFactor), m_terrainColour(colour)
{
	// Initialise memory in the heap
	m_vertSize = m_worldSize * m_worldSize * 6;
	m_indicesSize = (m_worldSize - 1) * (m_worldSize - 1) * 6;

	m_terrainVertices = new float[m_vertSize];
	m_terrainIndices = new unsigned int[m_indicesSize];

	//Generate the world
	TerrainInit();
}

TerrainGenerator::~TerrainGenerator()
{
	std::cout << "something happened here" << std::endl;
	delete[] m_terrainVertices;
	delete[] m_terrainIndices;
}

void TerrainGenerator::TerrainGeneration1D(const float octave, const float persistence)
{
}

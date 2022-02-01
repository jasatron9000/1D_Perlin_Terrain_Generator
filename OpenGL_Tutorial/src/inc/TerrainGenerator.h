#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <random>
#include <math.h>
#include "Color.h"
#include "ErrorHandler.h"
#include <GLFW/glfw3.h>

class PerlinNoise1D
{
private:
	unsigned int m_octave;
	float m_persistence;
	float m_normaliseMax;
	float m_maxHeight;

	unsigned int m_arraySize;

	float* m_noiseMap;
	float* m_perlinNoiseMap;

	// Callback
	void GenerateNoise();

	// Helper functions
	bool IsPowerOfTwo(unsigned int x);
	float NoiseFunction(const unsigned int index, const unsigned int arraySize, const float* sampledNoise);
	float CosineInterpolation(const float start, const float end, const float a);

public:
	// Generate the perlin noise
	PerlinNoise1D(const unsigned int arraySize, const unsigned int octave, const float persistence, const float maxHeight);
	~PerlinNoise1D();

	// Regenerate the noise
	void ResetNoise();
	void ResetNoise(const unsigned int arraySize); // Change size

	// Retrieve Noise Value
	inline float getNoise(const unsigned int x) { return m_perlinNoiseMap[x % m_arraySize]; }
};

class TerrainGenerator
{
private:
	unsigned int m_worldSize;
	float m_scalingFactor;
	RGB m_terrainColour;

	float* m_terrainVertices;
	unsigned int* m_terrainIndices;

	unsigned int m_vertSize;
	unsigned int m_indicesSize;

	RGB HeightColourPicker(const float height, const float heightMax);

public:
	TerrainGenerator(unsigned int worldSize, float scalingFactor, RGB colour);
	~TerrainGenerator();

	// Terrain Generation Algorithm
	void TerrainGeneration1D(const float octave, const float persistence);
	void TerrainInit();

	inline unsigned int GetVertSize() { return m_vertSize; }
	inline unsigned int GetIndicesSize() { return m_indicesSize; }

	inline float* GetVertPtr() { return m_terrainVertices; }
	inline unsigned int* GetIndicesPtr() { return m_terrainIndices; }
};
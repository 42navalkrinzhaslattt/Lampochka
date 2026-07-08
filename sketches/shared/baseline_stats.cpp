// baseline_stats.cpp - implementations for statistical helpers
#include "baseline_stats.h"
#include "config.h"
#include "baseline.h"
#include <stdlib.h>
#include <string.h>

uint8_t quickSelect(uint8_t* arr, int left, int right, int k) {
  if (left == right) return arr[left];
  int pivotIndex = left + (right - left) / 2;
  uint8_t pivotValue = arr[pivotIndex];
  uint8_t temp = arr[pivotIndex];
  arr[pivotIndex] = arr[right];
  arr[right] = temp;
  int storeIndex = left;
  for (int i = left; i < right; i++) {
    if (arr[i] < pivotValue) {
      temp = arr[storeIndex]; arr[storeIndex] = arr[i]; arr[i] = temp; storeIndex++;
    }
  }
  temp = arr[storeIndex]; arr[storeIndex] = arr[right]; arr[right] = temp;
  if (k == storeIndex) return arr[k];
  else if (k < storeIndex) return quickSelect(arr, left, storeIndex - 1, k);
  else return quickSelect(arr, storeIndex + 1, right, k);
}

float calculateMedian(uint8_t* data, int count) {
  if (count == 0) return 0;
  uint8_t* temp = (uint8_t*)malloc(count);
  if (!temp) return 0;
  memcpy(temp, data, count);
  float result;
  if (count % 2 == 0) {
    uint8_t mid1 = quickSelect(temp, 0, count - 1, count / 2 - 1);
    uint8_t mid2 = quickSelect(temp, 0, count - 1, count / 2);
    result = (mid1 + mid2) / 2.0;
  } else {
    result = quickSelect(temp, 0, count - 1, count / 2);
  }
  free(temp);
  return result;
}

float calculatePercentile(uint8_t* data, int count, float percentile) {
  if (count == 0) return 0;
  uint8_t* temp = (uint8_t*)malloc(count);
  if (!temp) return 0;
  memcpy(temp, data, count);
  float rank = (percentile / 100.0) * (count - 1);
  int lower = (int)rank;
  int upper = lower + 1;
  if (upper >= count) {
    float result = quickSelect(temp, 0, count - 1, count - 1);
    free(temp);
    return result;
  }
  uint8_t lowerVal = quickSelect(temp, 0, count - 1, lower);
  uint8_t upperVal = quickSelect(temp, 0, count - 1, upper);
  float result = lowerVal + (rank - lower) * (upperVal - lowerVal);
  free(temp);
  return result;
}

float calculateMean(uint8_t* data, int count) {
  if (count == 0) return 0;
  float sum = 0;
  for (int i = 0; i < count; i++) sum += data[i];
  return sum / count;
}

float calculateStd(uint8_t* data, int count, float mean) {
  if (count <= 1) return 0;
  float sumSquaredDiff = 0;
  for (int i = 0; i < count; i++) {
    float diff = data[i] - mean; sumSquaredDiff += diff * diff;
  }
  return sqrt(sumSquaredDiff / (count - 1));
}

float calculateMAD(uint8_t* data, int count, float median) {
  if (count == 0) return 0;
  uint8_t* deviations = (uint8_t*)malloc(count);
  if (!deviations) return 0;
  for (int i = 0; i < count; i++) deviations[i] = abs((int)data[i] - (int)median);
  float mad = calculateMedian(deviations, count);
  free(deviations);
  return mad * 1.4826;
}

float calculateMin(uint8_t* data, int count) {
  if (count == 0) return 0;
  uint8_t minVal = data[0];
  for (int i = 1; i < count; i++) if (data[i] < minVal) minVal = data[i];
  return minVal;
}

float calculateMax(uint8_t* data, int count) {
  if (count == 0) return 0;
  uint8_t maxVal = data[0];
  for (int i = 1; i < count; i++) if (data[i] > maxVal) maxVal = data[i];
  return maxVal;
}

int countValidSamples(uint8_t* data, int count, float median, float mad) {
  int validCount = 0; float threshold = MAD_THRESHOLD * mad;
  for (int i = 0; i < count; i++) {
    float deviation = abs((int)data[i] - (int)median);
    if (deviation <= threshold) validCount++;
  }
  return validCount;
}

int copyValidSamples(uint8_t* src, int count, float median, float mad, uint8_t* dest) {
  int validCount = 0; float threshold = MAD_THRESHOLD * mad;
  for (int i = 0; i < count; i++) {
    float deviation = abs((int)src[i] - (int)median);
    if (deviation <= threshold) dest[validCount++] = src[i];
  }
  return validCount;
}

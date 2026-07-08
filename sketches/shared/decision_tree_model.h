/*
  Decision Tree Model for Arduino

  Memory-optimized tree traversal with precomputed arrays.
  Generated from scikit-learn DecisionTreeClassifier.
*/
#ifndef DECISION_TREE_MODEL_H
#define DECISION_TREE_MODEL_H

const int NUM_FEATURES = 15;
const int NUM_CLASSES = 2;
const int DT_NUM_NODES = 3;

const char* class_names[] = {"human", "nothing"};

int16_t dt_left[3] = {1, -1, -1};
int16_t dt_right[3] = {2, -1, -1};
int16_t dt_feature[3] = {13, -1, -1};
float dt_threshold[3] = {1.170674, 0, 0};
int16_t dt_class[3] = {-1, 1, 0};

String evaluateDecisionTree(float features[]) {
  int node = 0;
  while (node >= 0 && node < DT_NUM_NODES) {
    int f = dt_feature[node];
    if (f < 0) {
      int c = dt_class[node];
      if (c >= 0 && c < NUM_CLASSES) return String(class_names[c]);
      return "unknown";
    }
    float thr = dt_threshold[node];
    if (features[f] <= thr) node = dt_left[node]; else node = dt_right[node];
    if (node < 0) break;
  }
  return "unknown";
}

#endif

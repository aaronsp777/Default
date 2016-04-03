//
//  main.c
//  DeepBeliefSDK
//
//  Created by Peter Warden on 4/28/14.
//  Copyright (c) 2014 Jetpac, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include <libjpcnn.h>

#define NETWORK_FILE_NAME "jetpac.ntwk"

#define CAMERA_TMP_FILE "/tmp/outfile.jpg"

FILE *camera_start() {
  char *cmd = "raspistill --preview 512,256,512,512 -vf -hf --width 256 --height 256 -k -t 100000 -o /tmp/outfile.jpg";
  return popen(cmd, "w");
}

int camera_stop(FILE *p) {
  fprintf(p, "x\n");
  fflush(p);
  return pclose(p);
}

char *camera_snapshot(FILE *p) {
  struct stat sb;
  unlink(CAMERA_TMP_FILE);
  printf("taking picture\n");
  fprintf(p, "\n");
  fflush(p);
  while(1) {
    // Polling every 100mS
    usleep(100 * 1000);
    if (stat(CAMERA_TMP_FILE, &sb)==0) {
      printf("found picture\n");
      return CAMERA_TMP_FILE;
    }
  }
}


int by_index_scores(const void *a, const void *b, void *scores_v) {
  float *scores = (float *)scores_v; 
  float diff = scores[*(int*)a] - scores[*(int*)b];
  if (diff < 0) {
    return 1;
  } else if (diff > 0) {
    return -1;
  } else {
    return 0;
  }
}

// Caller must free returned indexes
int *sort_indexes_by_scores(float *scores, int count) {
  int *indexes = calloc(count, sizeof(int));
  int i;

  if (indexes == 0) {
    fprintf(stderr, "calloc failed\n");
    exit(-1);
  }
  for (i=0; i<count; i++) {
    indexes[i] = i;
  } 
  qsort_r(indexes, count, sizeof(int), by_index_scores, scores);

  return indexes;
}

int main(int argc, const char * argv[]) {

  void* imageHandle;
  void* networkHandle;

  float* predictions;
  int predictionsLength;
  char** predictionsLabels;
  int predictionsLabelsLength;
  int index,i,snapcount;
  const float threshold = 0.01;
  FILE *cam;

  const char* networkFileName;
  if (argc < 3) {
    networkFileName = NETWORK_FILE_NAME;
  } else {
    networkFileName = argv[2];
  }

  cam = camera_start(); 

  networkHandle = jpcnn_create_network(networkFileName);
  if (networkHandle == NULL) {
    camera_stop(cam);
    fprintf(stderr, "DeepBeliefSDK: Couldn't load network file '%s'\n", networkFileName);
    return 1;
  }

  for (snapcount=0; snapcount<100; snapcount++) {

    imageHandle = jpcnn_create_image_buffer_from_file(camera_snapshot(cam));
    if (imageHandle == NULL) {
      camera_stop(cam);
      fprintf(stderr, "Couldn't load image file\n");
      return 1;
    }

    jpcnn_classify_image(networkHandle, imageHandle, 0, 0, &predictions, &predictionsLength, &predictionsLabels, &predictionsLabelsLength);
    jpcnn_destroy_image_buffer(imageHandle);

    int *sortedIndexes = sort_indexes_by_scores(predictions, predictionsLength);

    for (i = 0; i < predictionsLength; i++) {
      index = sortedIndexes[i];
      float predictionValue = predictions[index];
      char* label = predictionsLabels[index];
      if (predictionValue < threshold) {
        break;
      }
      printf("%f\t%s\n", predictionValue, label);
    }

    free(sortedIndexes);
  }

  camera_stop(cam);

  jpcnn_destroy_network(networkHandle);

  return 0;
}

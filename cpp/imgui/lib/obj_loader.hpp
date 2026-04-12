#include <iostream>

bool startsWith(const char *a, const char *b, int blen) {
   if(strncmp(a, b, blen) == 0) return 1;
   return 0;
}

float *loadObj(int *numOfVertices) {
    const char *path = "assets/cube.obj";

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int vBufLen = 0;
    int nBufLen = 0;
    int fBufLen = 0;
    float vBuf[1000];
    float nBuf[1000];
    int fBuf[1000][2];

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strncmp(line, "v ", 2) == 0) {
            char buf[10] = {0};
            int bi = 0;
            for (int i = 2; i < read; i++) {
                if (line[i] == ' ' || line[i] == '\n') {
                    buf[bi] = 0;
                    bi = 0;
                    vBuf[vBufLen] = atof(buf);
                    vBufLen++;
                    continue;
                }
                buf[bi] = line[i];
                bi++;
            }
        } else if (strncmp(line, "vn ", 3) == 0) {
            char buf[10] = {0};
            int bi = 0;
            for (int i = 3; i < read; i++) {
                if (line[i] == ' ' || line[i] == '\n') {
                    buf[bi] = 0;
                    bi = 0;
                    nBuf[nBufLen] = atof(buf);
                    nBufLen++;
                    continue;
                }
                buf[bi] = line[i];
                bi++;
            }
        } else if (strncmp(line, "f ", 2) == 0) {
            char buf[3][10] = {0};
            int bi = 0;
            int bj = 0;
            for (int i = 2; i < read; i++) {
                if (line[i] == ' ' || line[i] == '\n') {
                    buf[bi][bj] = 0;
                    fBuf[fBufLen][0] = atoi(buf[0]) - 1;
                    fBuf[fBufLen][1] = atoi(buf[2]) - 1;
                    fBufLen++;
                    bi = 0;
                    bj = 0;
                    continue;
                }
                if (line[i] == '/') {
                    buf[bi][bj] = 0;
                    bj = 0;
                    bi++;
                    continue;
                }
                buf[bi][bj] = line[i];
                bj++;
            }
        }
    }

    /**
     * 6 faces 
     * 12 triangles (2 triangles per face)
     * 36 vertices (3 vertices per triangle)
     * 216 float (6 floats per vertex)
     */
    int vDataBufLen = 0;
    int vStride = 3;
    int nStride = 3;
    int fStride = 4;
    int vDataBufStride = vStride + nStride;
    // int vDataBufStride = vStride;
    float vDataBuf[10000] = {0};
    for (int i = 0; i < fBufLen; i += fStride) {
        // face i
        int vi[fStride];
        int ni[fStride];
        int maxJ = i + fStride;
        for (int j = 0; j + i < maxJ; j++) {
            vi[j] = fBuf[j + i][0] * vStride;
            ni[j] = fBuf[j + i][1] * nStride;
        }
        // ----- triangle 1 --------
        int p1[3] = {0, 1, 2};
        for (int j = 0; j < 3; j++) {
            // vertex j
            int pi = p1[j];
            vDataBuf[vDataBufLen + 0] = vBuf[vi[pi] + 0];
            vDataBuf[vDataBufLen + 1] = vBuf[vi[pi] + 1];
            vDataBuf[vDataBufLen + 2] = vBuf[vi[pi] + 2];
            vDataBuf[vDataBufLen + 3] = nBuf[ni[pi] + 0];
            vDataBuf[vDataBufLen + 4] = nBuf[ni[pi] + 1];
            vDataBuf[vDataBufLen + 5] = nBuf[ni[pi] + 2];
            vDataBufLen += vDataBufStride;
        }

        // ----- triangle 2 --------
        int p2[3] = {0, 3, 2};
        for (int j = 0; j < 3; j++) {
            // vertex j
            int pi = p2[j];
            vDataBuf[vDataBufLen + 0] = vBuf[vi[pi] + 0];
            vDataBuf[vDataBufLen + 1] = vBuf[vi[pi] + 1];
            vDataBuf[vDataBufLen + 2] = vBuf[vi[pi] + 2];
            vDataBuf[vDataBufLen + 3] = nBuf[ni[pi] + 0];
            vDataBuf[vDataBufLen + 4] = nBuf[ni[pi] + 1];
            vDataBuf[vDataBufLen + 5] = nBuf[ni[pi] + 2];
            vDataBufLen += vDataBufStride;
        }
    }

    float *vData = (float *)malloc(vDataBufLen * sizeof(float));
    *numOfVertices = vDataBufLen / vDataBufStride;

    for (int i = 0; i < vDataBufLen; i ++) {
        vData[i] = vDataBuf[i];
    }

    free(line);

    return vData;
}
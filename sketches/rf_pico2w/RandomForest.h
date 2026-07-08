#pragma once

#include <math.h>

#include <string.h>
void add_vectors(double *v1, double *v2, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}
void mul_vector_number(double *v1, double num, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}
void score(double * input, double * output) {
    double var0[2];
    double var1[2];
    double var2[2];
    double var3[2];
    double var4[2];
    double var5[2];
    double var6[2];
    double var7[2];
    double var8[2];
    double var9[2];
    double var10[2];
    double var11[2];
    double var12[2];
    double var13[2];
    double var14[2];
    double var15[2];
    if (input[27] <= 0.9285500049591064) {
        if (input[1] <= 135.5) {
            if (input[27] <= 0.8716500103473663) {
                if (input[6] <= 307.0) {
                    if (input[0] <= 96.5) {
                        memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[19] <= 1.448900043964386) {
                            memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[19] <= 1.5004500150680542) {
                        if (input[17] <= 0.8481000065803528) {
                            if (input[29] <= 35.5) {
                                memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[4] <= 0.22110000252723694) {
                    if (input[21] <= 0.0929500013589859) {
                        memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[22] <= 5.816100120544434) {
                            if (input[22] <= 5.694999933242798) {
                                if (input[14] <= 851.9549865722656) {
                                    memcpy(var15, (double[]){0.9294117647058824, 0.07058823529411765}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.6818181818181818, 0.3181818181818182}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[6] <= 66.0) {
                                    memcpy(var15, (double[]){0.7142857142857143, 0.2857142857142857}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[14] <= 373.4556121826172) {
                        memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[1] <= 131.0) {
                            memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[23] <= 2.708400011062622) {
                memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
            } else {
                if (input[21] <= 0.6189500093460083) {
                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[23] <= 2.0411499738693237) {
            if (input[2] <= 1.5) {
                memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[14] <= 24.826400756835938) {
                    if (input[29] <= 62.0) {
                        if (input[9] <= 20.045000076293945) {
                            memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[0] <= 46.0) {
                                memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[0] <= 23.5) {
                            memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[0] <= 40.0) {
                        memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[4] <= 8.755000114440918) {
                            if (input[1] <= 110.0) {
                                if (input[20] <= 0.3149999976158142) {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[29] <= 84.0) {
                                if (input[14] <= 562.0942153930664) {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[3] <= 3.3736000061035156) {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            }
        } else {
            if (input[17] <= 0.8865500092506409) {
                if (input[1] <= 140.5) {
                    if (input[0] <= 45.5) {
                        memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[26] <= 8.97719955444336) {
                            if (input[22] <= 3.2450000047683716) {
                                if (input[25] <= 1.0850000381469727) {
                                    memcpy(var15, (double[]){0.4, 0.6}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.07692307692307693, 0.9230769230769231}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 103.5) {
                                    memcpy(var15, (double[]){0.9333333333333333, 0.06666666666666667}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.0967741935483871, 0.9032258064516129}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[8] <= 10.507999897003174) {
                                if (input[19] <= 1.4049999713897705) {
                                    memcpy(var15, (double[]){0.7738095238095238, 0.2261904761904762}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.9956709956709957, 0.004329004329004329}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[29] <= 122.0) {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[23] <= 2.607599973678589) {
                        if (input[9] <= 0.45499999821186066) {
                            if (input[23] <= 2.465000033378601) {
                                if (input[4] <= 0.3999999910593033) {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.2727272727272727, 0.7272727272727273}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[25] <= 1.2050000429153442) {
                                    memcpy(var15, (double[]){0.7777777777777778, 0.2222222222222222}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.15, 0.85}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[22] <= 5.519999980926514) {
                                if (input[4] <= 13.42585039138794) {
                                    memcpy(var15, (double[]){0.0016611295681063123, 0.9983388704318937}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[28] <= 4.5) {
                            if (input[24] <= 0.6547999978065491) {
                                if (input[2] <= 3.5) {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[22] <= 6.2288501262664795) {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[22] <= 5.669299840927124) {
                                memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[0] <= 98.5) {
                                    memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var15, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            } else {
                memcpy(var15, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        }
    }
    double var16[2];
    if (input[10] <= 21.5) {
        if (input[27] <= 0.9234000146389008) {
            if (input[16] <= 0.5) {
                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[27] <= 0.8799499869346619) {
                    if (input[0] <= 98.5) {
                        if (input[3] <= 1.7405999898910522) {
                            if (input[24] <= 1.3501499891281128) {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[9] <= 11.836099624633789) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.25, 0.75}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[5] <= 1.5) {
                                if (input[22] <= 5.945000171661377) {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[27] <= 0.8691999912261963) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.9166666666666666, 0.08333333333333333}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[19] <= 1.4918999671936035) {
                            memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[23] <= 2.6750000715255737) {
                        if (input[0] <= 84.5) {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[10] <= 15.0) {
                                if (input[0] <= 91.5) {
                                    memcpy(var16, (double[]){0.2857142857142857, 0.7142857142857143}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[6] <= 53.5) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[18] <= 0.3449999988079071) {
                if (input[4] <= 3.121350049972534) {
                    if (input[1] <= 139.5) {
                        if (input[9] <= 7.329999923706055) {
                            if (input[15] <= 617.8049926757812) {
                                if (input[8] <= 6.375) {
                                    memcpy(var16, (double[]){0.8277777777777777, 0.17222222222222222}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.9655172413793104, 0.034482758620689655}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[13] <= 268.69249725341797) {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[14] <= 334.86399841308594) {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[3] <= 2.8130500316619873) {
                                    memcpy(var16, (double[]){0.7692307692307693, 0.23076923076923078}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.9444444444444444, 0.05555555555555555}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[23] <= 2.5) {
                            if (input[24] <= 0.8707999885082245) {
                                if (input[29] <= 21.5) {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.01904761904761905, 0.9809523809523809}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[24] <= 1.153249979019165) {
                                if (input[19] <= 1.3050000071525574) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.20689655172413793, 0.7931034482758621}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[13] <= 511.62298583984375) {
                                    memcpy(var16, (double[]){0.8888888888888888, 0.1111111111111111}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[0] <= 82.5) {
                        memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[19] <= 1.4785500168800354) {
                            memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[0] <= 47.5) {
                    if (input[21] <= 0.895499974489212) {
                        memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[23] <= 2.0799999237060547) {
                            if (input[21] <= 0.9053999781608582) {
                                memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[8] <= 0.6495500206947327) {
                        if (input[4] <= 0.5450000166893005) {
                            if (input[13] <= 4.320000052452087) {
                                memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[1] <= 137.5) {
                            if (input[0] <= 93.5) {
                                if (input[14] <= 280.55025482177734) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.16666666666666666, 0.8333333333333334}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[23] <= 2.6895999908447266) {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[10] <= -130.5) {
                                if (input[15] <= 69.62999725341797) {
                                    memcpy(var16, (double[]){0.0625, 0.9375}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.6, 0.4}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[19] <= 1.3651500344276428) {
                                    memcpy(var16, (double[]){0.01050420168067227, 0.9894957983193278}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (input[1] <= 136.5) {
            if (input[19] <= 1.3149999976158142) {
                if (input[12] <= 0.5) {
                    if (input[0] <= 59.5) {
                        if (input[8] <= 2.7549999952316284) {
                            if (input[8] <= 1.0) {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[23] <= 2.0449999570846558) {
                                memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[29] <= 71.5) {
                        if (input[18] <= 3.900149941444397) {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[14] <= 730.5050048828125) {
                                if (input[25] <= 1.47079998254776) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.25, 0.75}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[3] <= 6.049999952316284) {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[0] <= 57.0) {
                            if (input[19] <= 1.0913000106811523) {
                                memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[2] <= 2.5) {
                                if (input[16] <= 0.5) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[18] <= 4.164999961853027) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            } else {
                if (input[22] <= 6.182850122451782) {
                    if (input[15] <= 51.260000228881836) {
                        memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[22] <= 6.147850036621094) {
                            if (input[1] <= 132.5) {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[4] <= 0.2149999961256981) {
                                    memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[20] <= 1.3547500371932983) {
                if (input[19] <= 1.3602499961853027) {
                    if (input[3] <= 8.874199867248535) {
                        if (input[1] <= 302.5) {
                            if (input[27] <= 2.024999976158142) {
                                if (input[26] <= 9.038150310516357) {
                                    memcpy(var16, (double[]){0.003155569580309246, 0.9968444304196907}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.15714285714285714, 0.8428571428571429}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[29] <= 91.5) {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[27] <= 0.9749999940395355) {
                        if (input[28] <= 10.0) {
                            memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 478.1025085449219) {
                            if (input[3] <= 2.012500047683716) {
                                memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[6] <= 100.0) {
                                memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[3] <= 2.2412999868392944) {
                                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var16, (double[]){0.1111111111111111, 0.8888888888888888}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            } else {
                if (input[28] <= 6.0) {
                    if (input[13] <= 477.31800842285156) {
                        memcpy(var16, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var16, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var15, var16, 2, var14);
    double var17[2];
    if (input[27] <= 0.9285500049591064) {
        if (input[27] <= 0.8608500063419342) {
            if (input[7] <= 12.5) {
                memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[19] <= 1.485450029373169) {
                    if (input[20] <= -0.5048500150442123) {
                        memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[14] <= 542.6535949707031) {
                if (input[4] <= 0.22145000100135803) {
                    if (input[5] <= 42.0) {
                        memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[0] <= 80.0) {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[11] <= -29.0) {
                                memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[11] <= 17.5) {
                                    memcpy(var17, (double[]){0.08, 0.92}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[0] <= 86.0) {
                        memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[23] <= 2.722249984741211) {
                            memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[1] <= 133.5) {
                    if (input[1] <= 132.5) {
                        if (input[15] <= 72.69499969482422) {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[15] <= 73.82499694824219) {
                                memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[6] <= 21.5) {
                                    memcpy(var17, (double[]){0.9090909090909091, 0.09090909090909091}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[8] <= 4.494999885559082) {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[26] <= 9.951000213623047) {
                        memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[0] <= 50.5) {
            if (input[1] <= 85.5) {
                memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[21] <= 0.0823500007390976) {
                    memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[22] <= 2.415000081062317) {
                        memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[27] <= 1.0613000094890594) {
                            memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[1] <= 141.5) {
                if (input[26] <= 9.735249996185303) {
                    if (input[4] <= 0.2862499952316284) {
                        if (input[22] <= 3.2447999715805054) {
                            memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[23] <= 2.490000009536743) {
                                if (input[1] <= 102.0) {
                                    memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){0.14285714285714285, 0.8571428571428571}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[27] <= 1.0199999809265137) {
                                    memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[27] <= 1.150950014591217) {
                            if (input[7] <= 11.5) {
                                if (input[13] <= 722.2454833984375) {
                                    memcpy(var17, (double[]){0.08333333333333333, 0.9166666666666666}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[26] <= 7.2200000286102295) {
                                memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[6] <= 593.0) {
                                    memcpy(var17, (double[]){0.9767441860465116, 0.023255813953488372}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[27] <= 1.8350000381469727) {
                    if (input[0] <= 69.5) {
                        if (input[8] <= 4.25094997882843) {
                            memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[4] <= 15.683550357818604) {
                            if (input[10] <= -109.5) {
                                if (input[22] <= 5.200000047683716) {
                                    memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){0.9285714285714286, 0.07142857142857142}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[22] <= 5.335000038146973) {
                                    memcpy(var17, (double[]){0.0022219136231079015, 0.9977780863768921}, 2 * sizeof(double));
                                } else {
                                    memcpy(var17, (double[]){0.6708860759493671, 0.3291139240506329}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[23] <= 1.6133999824523926) {
                        memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[13] <= 183.50250244140625) {
                            memcpy(var17, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var17, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var14, var17, 2, var13);
    double var18[2];
    if (input[0] <= 92.5) {
        if (input[27] <= 0.9291000068187714) {
            if (input[10] <= 13.5) {
                if (input[17] <= 0.8850000202655792) {
                    if (input[22] <= 4.769099950790405) {
                        if (input[18] <= 4.3450000286102295) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[0] <= 87.5) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 5.460000038146973) {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[13] <= 739.2460021972656) {
                    if (input[4] <= 0.2175000011920929) {
                        memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[19] <= 1.3650000095367432) {
                            if (input[19] <= 1.3550000190734863) {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[23] <= 2.5309499502182007) {
                        memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[1] <= 99.5) {
                if (input[1] <= 84.5) {
                    if (input[10] <= -107.5) {
                        if (input[28] <= 10.5) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[10] <= -110.0) {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[22] <= 2.6449999809265137) {
                        if (input[21] <= 0.016200000420212746) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[3] <= 4.019999980926514) {
                                if (input[16] <= 0.5) {
                                    memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[25] <= 1.1200000047683716) {
                            memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 2.8149999380111694) {
                                if (input[10] <= -67.0) {
                                    memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            } else {
                if (input[6] <= 11.5) {
                    if (input[22] <= 4.370000123977661) {
                        if (input[19] <= 1.649999976158142) {
                            if (input[24] <= 0.7450000047683716) {
                                if (input[0] <= 76.0) {
                                    memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[3] <= 1.800000011920929) {
                            memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[22] <= 5.288349866867065) {
                        if (input[4] <= 5.630000114440918) {
                            if (input[19] <= 1.3880500197410583) {
                                if (input[26] <= 8.70609998703003) {
                                    memcpy(var18, (double[]){0.004878048780487805, 0.9951219512195122}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.13333333333333333, 0.8666666666666667}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[23] <= 1.9543499946594238) {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[22] <= 5.799600124359131) {
            if (input[4] <= 14.992500305175781) {
                if (input[22] <= 5.7149999141693115) {
                    if (input[26] <= 4.5) {
                        if (input[9] <= 19.12975025177002) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[23] <= 2.3996500968933105) {
                            if (input[19] <= 1.3849999904632568) {
                                if (input[27] <= 1.8350000381469727) {
                                    memcpy(var18, (double[]){0.00014639145073927682, 0.9998536085492608}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.5, 0.5}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[27] <= 1.3350000381469727) {
                                if (input[6] <= 5.5) {
                                    memcpy(var18, (double[]){0.03125, 0.96875}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[29] <= 130.5) {
                                    memcpy(var18, (double[]){0.8461538461538461, 0.15384615384615385}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[7] <= 8.0) {
                        if (input[22] <= 5.734999895095825) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[26] <= 9.885000228881836) {
                                if (input[1] <= 143.5) {
                                    memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        } else {
            if (input[7] <= 8.5) {
                if (input[23] <= 2.6892499923706055) {
                    if (input[15] <= 100.8781509399414) {
                        if (input[26] <= 9.934999942779541) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 203.19499969482422) {
                            memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[26] <= 10.142149925231934) {
                        if (input[19] <= 1.4414499998092651) {
                            if (input[4] <= 1.806850016117096) {
                                if (input[7] <= 7.5) {
                                    memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.9, 0.1}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[19] <= 1.4487000107765198) {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[1] <= 136.5) {
                    if (input[1] <= 130.5) {
                        memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[24] <= 1.3949999809265137) {
                            if (input[27] <= 0.8280499875545502) {
                                if (input[14] <= 399.00856018066406) {
                                    memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[13] <= 229.49800491333008) {
                                memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[22] <= 6.170849800109863) {
                        memcpy(var18, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var18, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var13, var18, 2, var12);
    double var19[2];
    if (input[1] <= 136.5) {
        if (input[17] <= 0.8850000202655792) {
            if (input[26] <= 9.312049865722656) {
                if (input[17] <= 0.8481000065803528) {
                    if (input[29] <= 61.5) {
                        if (input[14] <= 295.7379608154297) {
                            if (input[29] <= 53.0) {
                                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[25] <= 1.4789000153541565) {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[0] <= 43.5) {
                                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[4] <= 0.23900000005960464) {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.2, 0.8}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[27] <= 1.185800015926361) {
                            if (input[1] <= 101.5) {
                                if (input[20] <= -0.2905000001192093) {
                                    memcpy(var19, (double[]){0.3333333333333333, 0.6666666666666666}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.9705882352941176, 0.029411764705882353}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[18] <= 3.920899987220764) {
                                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[4] <= 1.5444999933242798) {
                                    memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[22] <= 2.774999976158142) {
                        if (input[1] <= 85.5) {
                            memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[15] <= 1215.0050048828125) {
                                memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[1] <= 100.5) {
                            memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 4.555000066757202) {
                                if (input[4] <= 0.42499999701976776) {
                                    memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.875, 0.125}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            } else {
                if (input[4] <= 0.19210000336170197) {
                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[4] <= 0.19250000268220901) {
                        memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[19] <= 1.413349986076355) {
                            if (input[22] <= 4.8999998569488525) {
                                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[18] <= 4.384999990463257) {
                                    memcpy(var19, (double[]){0.9490445859872612, 0.050955414012738856}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.625, 0.375}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[4] <= 0.19995000213384628) {
                                if (input[19] <= 1.4480500221252441) {
                                    memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            }
        } else {
            memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
        }
    } else {
        if (input[27] <= 1.7931999564170837) {
            if (input[19] <= 1.3049999475479126) {
                if (input[14] <= 20.462149620056152) {
                    if (input[10] <= 33.5) {
                        memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[26] <= 8.52584981918335) {
                        if (input[20] <= 0.9799500107765198) {
                            if (input[22] <= 4.770000219345093) {
                                memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[26] <= 8.385000228881836) {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[19] <= 1.1899999976158142) {
                                memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[26] <= 8.295000076293945) {
                                    memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[4] <= 0.32500000298023224) {
                            memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 5.270000219345093) {
                                memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[7] <= 3.5) {
                                    memcpy(var19, (double[]){0.6470588235294118, 0.35294117647058826}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            } else {
                if (input[18] <= 4.555000066757202) {
                    if (input[1] <= 160.5) {
                        if (input[2] <= 2.5) {
                            if (input[8] <= 10.764500141143799) {
                                memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[9] <= 7.611449956893921) {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[27] <= 1.1365500092506409) {
                                if (input[22] <= 5.812900066375732) {
                                    memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.6470588235294118, 0.35294117647058826}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[17] <= 0.7307500243186951) {
                                    memcpy(var19, (double[]){0.5714285714285714, 0.42857142857142855}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){0.9491525423728814, 0.05084745762711865}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[23] <= 2.7134000062942505) {
                        if (input[2] <= 3.5) {
                            if (input[27] <= 0.9493999779224396) {
                                memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[26] <= 9.651600360870361) {
                                    memcpy(var19, (double[]){0.1, 0.9}, 2 * sizeof(double));
                                } else {
                                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[1] <= 284.0) {
                if (input[13] <= 183.50250244140625) {
                    if (input[22] <= 1.0600000023841858) {
                        memcpy(var19, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var19, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var12, var19, 2, var11);
    double var20[2];
    if (input[1] <= 136.5) {
        if (input[27] <= 0.8500500023365021) {
            if (input[27] <= 0.8260499835014343) {
                memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[11] <= -9.0) {
                    if (input[20] <= -0.35190001130104065) {
                        memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[29] <= 32.5) {
                if (input[29] <= 20.5) {
                    if (input[8] <= 2.725600004196167) {
                        if (input[0] <= 41.0) {
                            memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[26] <= 9.088099956512451) {
                                memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[0] <= 59.5) {
                            memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 4.1164000034332275) {
                                memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[4] <= 0.2549999952316284) {
                        if (input[6] <= 473.0) {
                            if (input[26] <= 9.78499984741211) {
                                if (input[3] <= 3.975000023841858) {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[18] <= 3.921899914741516) {
                    if (input[18] <= 0.34654998779296875) {
                        if (input[1] <= 87.5) {
                            if (input[28] <= 10.5) {
                                if (input[15] <= 690.6317443847656) {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.9642857142857143, 0.03571428571428571}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[6] <= 300.5) {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[19] <= 1.2949999570846558) {
                                if (input[27] <= 1.312600016593933) {
                                    memcpy(var20, (double[]){0.027777777777777776, 0.9722222222222222}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 132.5) {
                                    memcpy(var20, (double[]){0.9612903225806452, 0.03870967741935484}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.7608695652173914, 0.2391304347826087}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[0] <= 47.5) {
                            if (input[10] <= 63.5) {
                                memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[28] <= 3.5) {
                                    memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[18] <= 3.9009000062942505) {
                                memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[0] <= 49.5) {
                        memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[2] <= 2.5) {
                            if (input[4] <= 0.22110000252723694) {
                                if (input[10] <= 90.5) {
                                    memcpy(var20, (double[]){0.8409090909090909, 0.1590909090909091}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.25, 0.75}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[20] <= -0.3641500025987625) {
                                    memcpy(var20, (double[]){0.6, 0.4}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.1076923076923077, 0.8923076923076924}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[22] <= 2.78000009059906) {
                                if (input[7] <= 5.0) {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[25] <= 1.847350001335144) {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.8571428571428571, 0.14285714285714285}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (input[22] <= 5.7149999141693115) {
            if (input[4] <= 7.039999961853027) {
                if (input[19] <= 1.425000011920929) {
                    if (input[27] <= 1.9700000286102295) {
                        if (input[19] <= 1.1696999669075012) {
                            if (input[22] <= 4.305000066757202) {
                                memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[27] <= 1.409999966621399) {
                                    memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[10] <= 99.5) {
                                if (input[4] <= 0.3449999988079071) {
                                    memcpy(var20, (double[]){0.001941747572815534, 0.9980582524271845}, 2 * sizeof(double));
                                } else {
                                    memcpy(var20, (double[]){0.22123893805309736, 0.7787610619469026}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[3] <= 3.326150059700012) {
                    memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[13] <= 515.4325103759766) {
                if (input[2] <= 2.5) {
                    if (input[3] <= 3.715000033378601) {
                        memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[27] <= 0.8608500063419342) {
                            memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[27] <= 0.9800000190734863) {
                        if (input[28] <= 8.0) {
                            memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[13] <= 610.7455139160156) {
                    if (input[23] <= 2.812399983406067) {
                        memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[25] <= 1.7607499957084656) {
                        if (input[27] <= 1.0960500240325928) {
                            memcpy(var20, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var20, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var11, var20, 2, var10);
    double var21[2];
    if (input[3] <= 2.246150016784668) {
        if (input[1] <= 136.5) {
            if (input[27] <= 0.8258500099182129) {
                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[27] <= 0.8280499875545502) {
                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 132.5) {
                        if (input[10] <= 91.0) {
                            if (input[4] <= 2.971750020980835) {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[11] <= 11.5) {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[13] <= 305.3459930419922) {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[22] <= 6.2507500648498535) {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[11] <= 1.5) {
                            if (input[15] <= 66.61124992370605) {
                                memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[1] <= 133.5) {
                                memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            }
        } else {
            if (input[19] <= 1.4473000168800354) {
                if (input[3] <= 2.0939499139785767) {
                    if (input[9] <= 0.5) {
                        if (input[22] <= 5.055000066757202) {
                            memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[24] <= 1.5950000286102295) {
                            if (input[22] <= 5.681299924850464) {
                                memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[11] <= 19.5) {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[26] <= 7.990250110626221) {
                        memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[8] <= 6.848999977111816) {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        }
    } else {
        if (input[3] <= 5.658349990844727) {
            if (input[27] <= 0.9287000000476837) {
                if (input[23] <= 2.6850000619888306) {
                    if (input[22] <= 4.825000047683716) {
                        if (input[19] <= 1.3316500186920166) {
                            if (input[13] <= 214.16000366210938) {
                                if (input[22] <= 4.769999980926514) {
                                    memcpy(var21, (double[]){0.125, 0.875}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.875, 0.125}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 130.0) {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[26] <= 9.565700054168701) {
                            if (input[0] <= 81.5) {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[3] <= 4.494999885559082) {
                                if (input[13] <= 70.33000183105469) {
                                    memcpy(var21, (double[]){0.07142857142857142, 0.9285714285714286}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.5882352941176471, 0.4117647058823529}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[25] <= 1.5700000524520874) {
                                    memcpy(var21, (double[]){0.9259259259259259, 0.07407407407407407}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.3333333333333333, 0.6666666666666666}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[8] <= 3.186850070953369) {
                        if (input[26] <= 10.047299861907959) {
                            if (input[26] <= 10.007299900054932) {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[14] <= 363.722900390625) {
                    if (input[1] <= 136.5) {
                        if (input[1] <= 83.5) {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[23] <= 2.3968499898910522) {
                                if (input[25] <= 1.118299961090088) {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[6] <= 200.0) {
                            if (input[23] <= 2.819849967956543) {
                                if (input[19] <= 1.5117499828338623) {
                                    memcpy(var21, (double[]){0.004098360655737705, 0.9959016393442623}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[20] <= 0.5426499843597412) {
                                if (input[14] <= 108.45780181884766) {
                                    memcpy(var21, (double[]){0.2222222222222222, 0.7777777777777778}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[1] <= 142.5) {
                        if (input[2] <= 2.5) {
                            if (input[0] <= 47.5) {
                                if (input[15] <= 8.764999866485596) {
                                    memcpy(var21, (double[]){0.9333333333333333, 0.06666666666666667}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.9945945945945946, 0.005405405405405406}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[22] <= 4.2581000328063965) {
                                    memcpy(var21, (double[]){0.10101010101010101, 0.898989898989899}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.5714285714285714, 0.42857142857142855}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[13] <= 623.5455017089844) {
                                if (input[18] <= 3.915000081062317) {
                                    memcpy(var21, (double[]){0.9594594594594594, 0.04054054054054054}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.775, 0.225}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[20] <= 0.11235000193119049) {
                                    memcpy(var21, (double[]){0.2, 0.8}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[1] <= 164.5) {
                            if (input[19] <= 1.3149999976158142) {
                                if (input[2] <= 2.5) {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.26811594202898553, 0.7318840579710145}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[28] <= 6.5) {
                                    memcpy(var21, (double[]){0.21739130434782608, 0.782608695652174}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.7586206896551724, 0.2413793103448276}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[29] <= 110.5) {
                                if (input[21] <= 0.10500000044703484) {
                                    memcpy(var21, (double[]){0.0014492753623188406, 0.9985507246376811}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[16] <= 0.5) {
                                    memcpy(var21, (double[]){0.2777777777777778, 0.7222222222222222}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.002207505518763797, 0.9977924944812362}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            }
        } else {
            if (input[10] <= 72.5) {
                if (input[2] <= 2.5) {
                    if (input[26] <= 9.97100019454956) {
                        if (input[2] <= 1.5) {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[3] <= 6.644999980926514) {
                                if (input[13] <= 242.447998046875) {
                                    memcpy(var21, (double[]){0.4072164948453608, 0.5927835051546392}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.7927927927927928, 0.2072072072072072}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[27] <= 1.0049999952316284) {
                                    memcpy(var21, (double[]){0.9, 0.1}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[9] <= 5.284049987792969) {
                        if (input[19] <= 1.284999966621399) {
                            if (input[17] <= 0.8100000023841858) {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[10] <= -89.5) {
                                    memcpy(var21, (double[]){0.4, 0.6}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[0] <= 94.5) {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[4] <= 0.22999999672174454) {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[20] <= 0.33550000190734863) {
                            if (input[26] <= 8.59499979019165) {
                                if (input[0] <= 71.0) {
                                    memcpy(var21, (double[]){0.75, 0.25}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){0.08571428571428572, 0.9142857142857143}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[14] <= 470.65684509277344) {
                    if (input[27] <= 0.88960000872612) {
                        memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[2] <= 3.5) {
                            if (input[1] <= 103.5) {
                                memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[22] <= 6.100199937820435) {
                                    memcpy(var21, (double[]){0.06, 0.94}, 2 * sizeof(double));
                                } else {
                                    memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[22] <= 5.619999885559082) {
                        if (input[27] <= 1.8215500712394714) {
                            memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[2] <= 3.5) {
                            memcpy(var21, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var21, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var10, var21, 2, var9);
    double var22[2];
    if (input[0] <= 92.5) {
        if (input[4] <= 0.21970000118017197) {
            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[0] <= 55.5) {
                if (input[14] <= 618.812744140625) {
                    if (input[5] <= 98.5) {
                        if (input[6] <= 308.5) {
                            if (input[24] <= 0.45794999599456787) {
                                if (input[13] <= 729.2454833984375) {
                                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[0] <= 51.5) {
                                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.9090909090909091, 0.09090909090909091}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[15] <= 165.6126937866211) {
                                memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[11] <= -6.5) {
                            if (input[28] <= 10.0) {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[19] <= 1.024999976158142) {
                        if (input[22] <= 2.1750000715255737) {
                            if (input[11] <= 9.5) {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[19] <= 0.7549999952316284) {
                                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[0] <= 45.5) {
                            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[27] <= 0.9850000143051147) {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[28] <= 4.5) {
                                    memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.65, 0.35}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            } else {
                if (input[27] <= 0.9391500055789948) {
                    if (input[27] <= 0.8817499876022339) {
                        memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[29] <= 116.5) {
                            if (input[6] <= 37.5) {
                                if (input[18] <= 4.329999923706055) {
                                    memcpy(var22, (double[]){0.7777777777777778, 0.2222222222222222}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[10] <= -58.0) {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[22] <= 4.3450000286102295) {
                        if (input[19] <= 1.3700000047683716) {
                            if (input[24] <= 0.8199999928474426) {
                                if (input[10] <= 105.5) {
                                    memcpy(var22, (double[]){0.07692307692307693, 0.9230769230769231}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 95.5) {
                                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.01107011070110701, 0.988929889298893}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[3] <= 1.8123500347137451) {
                            if (input[2] <= 2.5) {
                                memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[8] <= 7.838500022888184) {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[6] <= 50.0) {
                                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.3333333333333333, 0.6666666666666666}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (input[2] <= 1.5) {
            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
        } else {
            if (input[23] <= 2.6895999908447266) {
                if (input[4] <= 13.585950374603271) {
                    if (input[23] <= 2.65500009059906) {
                        if (input[23] <= 1.1500000059604645) {
                            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[10] <= -109.5) {
                                if (input[23] <= 2.570050001144409) {
                                    memcpy(var22, (double[]){0.022727272727272728, 0.9772727272727273}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.5, 0.5}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[27] <= 1.7949999570846558) {
                                    memcpy(var22, (double[]){0.0030079299972655183, 0.9969920700027345}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.3, 0.7}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[21] <= 0.7000000178813934) {
                            if (input[26] <= 9.476600170135498) {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[10] <= 15.5) {
                                    memcpy(var22, (double[]){0.7647058823529411, 0.23529411764705882}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[13] <= 304.22349548339844) {
                                memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[24] <= 1.3422499895095825) {
                                    memcpy(var22, (double[]){0.125, 0.875}, 2 * sizeof(double));
                                } else {
                                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[19] <= 1.4473000168800354) {
                    if (input[0] <= 98.5) {
                        if (input[29] <= 134.0) {
                            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[24] <= 1.1157000064849854) {
                                memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[22] <= 6.375149965286255) {
                            memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[24] <= 1.4731000065803528) {
                        memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[23] <= 2.816100001335144) {
                            if (input[25] <= 1.2217999696731567) {
                                memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var22, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var22, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var9, var22, 2, var8);
    double var23[2];
    if (input[1] <= 136.5) {
        if (input[10] <= 95.5) {
            if (input[27] <= 0.9234000146389008) {
                if (input[27] <= 0.8698500096797943) {
                    if (input[1] <= 130.5) {
                        memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[17] <= 0.8481000065803528) {
                            if (input[0] <= 97.5) {
                                memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[20] <= 0.10135000199079514) {
                        if (input[0] <= 79.5) {
                            memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[20] <= -0.07569999992847443) {
                                if (input[14] <= 720.5) {
                                    memcpy(var23, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 129.5) {
                                    memcpy(var23, (double[]){0.8571428571428571, 0.14285714285714285}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.125, 0.875}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[29] <= 33.5) {
                    if (input[15] <= 921.6300048828125) {
                        if (input[9] <= 3.9040499925613403) {
                            if (input[16] <= 0.5) {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[28] <= 7.5) {
                                    memcpy(var23, (double[]){0.9859154929577465, 0.014084507042253521}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.7777777777777778, 0.2222222222222222}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[1] <= 99.0) {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[23] <= 2.399999976158142) {
                                    memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[15] <= 926.6053161621094) {
                            memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[14] <= 519.0736083984375) {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[10] <= 18.5) {
                                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[16] <= 0.5) {
                        if (input[11] <= -23.5) {
                            if (input[3] <= 4.683850049972534) {
                                memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= -0.5) {
                            if (input[23] <= 2.343600034713745) {
                                if (input[18] <= 3.8709999322891235) {
                                    memcpy(var23, (double[]){0.67, 0.33}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.2619047619047619, 0.7380952380952381}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 112.5) {
                                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.7878787878787878, 0.21212121212121213}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[1] <= 85.5) {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[23] <= 2.4357500076293945) {
                                    memcpy(var23, (double[]){0.17857142857142858, 0.8214285714285714}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.9848484848484849, 0.015151515151515152}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            }
        } else {
            if (input[3] <= 6.320000171661377) {
                if (input[4] <= 0.19494999945163727) {
                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[19] <= 1.3050000071525574) {
                        memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[15] <= 1049.083740234375) {
                            if (input[4] <= 0.19995000213384628) {
                                memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[26] <= 9.875) {
                                    memcpy(var23, (double[]){0.8666666666666667, 0.13333333333333333}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[16] <= 0.5) {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            } else {
                if (input[4] <= 0.2326500043272972) {
                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[27] <= 1.1826000213623047) {
                        if (input[22] <= 5.195250034332275) {
                            memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[19] <= 1.3585500121116638) {
            if (input[29] <= 17.5) {
                if (input[11] <= 2.5) {
                    memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[22] <= 5.580000162124634) {
                        memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[2] <= 4.5) {
                    if (input[22] <= 5.234999895095825) {
                        if (input[27] <= 1.8710500001907349) {
                            if (input[22] <= 4.756050109863281) {
                                if (input[19] <= 1.31659996509552) {
                                    memcpy(var23, (double[]){0.00043449923962633063, 0.9995655007603736}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.021739130434782608, 0.9782608695652174}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[27] <= 1.350000023841858) {
                                    memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.9166666666666666, 0.08333333333333333}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[4] <= 0.2849999964237213) {
                            memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[3] <= 5.134999990463257) {
                                if (input[15] <= 3.8200000524520874) {
                                    memcpy(var23, (double[]){0.7142857142857143, 0.2857142857142857}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.972972972972973, 0.02702702702702703}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[29] <= 80.0) {
                                    memcpy(var23, (double[]){0.75, 0.25}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[3] <= 4.1447001695632935) {
                        if (input[27] <= 2.0482999682426453) {
                            memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[27] <= 1.0342999696731567) {
                if (input[24] <= 0.6938999891281128) {
                    if (input[14] <= 77.0665512084961) {
                        memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[8] <= 3.1049998998641968) {
                        if (input[22] <= 5.799299955368042) {
                            memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[23] <= 2.708400011062622) {
                            if (input[27] <= 1.0250999927520752) {
                                memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[24] <= 1.0248000025749207) {
                                    memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[22] <= 6.00314998626709) {
                                memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            } else {
                if (input[27] <= 1.1365500092506409) {
                    if (input[14] <= 360.0838928222656) {
                        memcpy(var23, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var23, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var8, var23, 2, var7);
    double var24[2];
    if (input[14] <= 512.1588745117188) {
        if (input[27] <= 0.9202499985694885) {
            if (input[1] <= 136.5) {
                if (input[0] <= 87.5) {
                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[7] <= 12.5) {
                        if (input[23] <= 2.6811500787734985) {
                            memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 6.56725001335144) {
                                if (input[24] <= 1.3949999809265137) {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.5, 0.5}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
            }
        } else {
            if (input[1] <= 139.5) {
                if (input[10] <= 91.5) {
                    if (input[14] <= 296.6664581298828) {
                        if (input[5] <= 2.5) {
                            if (input[1] <= 86.5) {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[22] <= 5.664999961853027) {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[10] <= 75.0) {
                                if (input[15] <= 906.6375427246094) {
                                    memcpy(var24, (double[]){0.9992251065478497, 0.0007748934521503294}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.6666666666666666, 0.3333333333333333}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[5] <= 30.0) {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[1] <= 81.0) {
                            memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[19] <= 1.2753999829292297) {
                                if (input[14] <= 414.7784881591797) {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.5, 0.5}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 135.5) {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.75, 0.25}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[26] <= 9.220000267028809) {
                        if (input[13] <= 63.82999897003174) {
                            if (input[13] <= 2.2099998965859413) {
                                if (input[4] <= 10.234999768435955) {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[9] <= 13.111550331115723) {
                                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[19] <= 1.1975499987602234) {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[4] <= 15.683550357818604) {
                    if (input[14] <= 509.5782012939453) {
                        if (input[26] <= 9.3986496925354) {
                            if (input[23] <= 2.5548499822616577) {
                                if (input[10] <= 157.5) {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.008264462809917356, 0.9917355371900827}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[13] <= 301.74500274658203) {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.75, 0.25}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[4] <= 0.7652000188827515) {
                                if (input[27] <= 1.057099997997284) {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[8] <= 8.249599933624268) {
                            memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        }
    } else {
        if (input[17] <= 0.925000011920929) {
            if (input[27] <= 0.8930000066757202) {
                if (input[25] <= 1.1119000315666199) {
                    if (input[23] <= 2.6450001001358032) {
                        memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 69.5) {
                        if (input[19] <= 1.4149999618530273) {
                            if (input[13] <= 7.800000071525574) {
                                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[23] <= 2.6401000022888184) {
                                    memcpy(var24, (double[]){0.7096774193548387, 0.2903225806451613}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.8823529411764706, 0.11764705882352941}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[22] <= 5.870000123977661) {
                                if (input[14] <= 946.875) {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[1] <= 136.0) {
                            memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[1] <= 136.5) {
                    if (input[22] <= 4.079999923706055) {
                        if (input[17] <= 0.8481000065803528) {
                            if (input[1] <= 116.5) {
                                if (input[17] <= 0.7307500243186951) {
                                    memcpy(var24, (double[]){0.9, 0.1}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[18] <= 3.9900001287460327) {
                                if (input[1] <= 85.0) {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.18055555555555555, 0.8194444444444444}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[24] <= 1.0950000286102295) {
                                    memcpy(var24, (double[]){0.4444444444444444, 0.5555555555555556}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.02702702702702703, 0.972972972972973}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[22] <= 5.7200000286102295) {
                            if (input[9] <= 1.574999988079071) {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[10] <= 101.0) {
                                    memcpy(var24, (double[]){0.891566265060241, 0.10843373493975904}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[23] <= 2.625) {
                        if (input[13] <= 640.4449768066406) {
                            if (input[19] <= 1.4150000214576721) {
                                if (input[22] <= -0.31655000895261765) {
                                    memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var24, (double[]){0.0031318509238960224, 0.996868149076104}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[8] <= 6.960400104522705) {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[22] <= 5.755000114440918) {
                            if (input[27] <= 1.1949999928474426) {
                                memcpy(var24, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            memcpy(var24, (double[]){1.0, 0.0}, 2 * sizeof(double));
        }
    }
    add_vectors(var7, var24, 2, var6);
    double var25[2];
    if (input[16] <= 0.5) {
        if (input[1] <= 139.5) {
            if (input[17] <= 0.8080999851226807) {
                if (input[1] <= 127.5) {
                    if (input[3] <= 1.6624499559402466) {
                        if (input[26] <= 9.623950004577637) {
                            memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 85.5) {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[24] <= 1.4950000047683716) {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        } else {
            if (input[20] <= 0.35074999928474426) {
                if (input[10] <= -107.0) {
                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[10] <= 42.0) {
                        if (input[1] <= 144.5) {
                            if (input[2] <= 4.0) {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[22] <= 5.960050106048584) {
                            if (input[9] <= 24.130000114440918) {
                                if (input[27] <= 2.0103999972343445) {
                                    memcpy(var25, (double[]){0.022727272727272728, 0.9772727272727273}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[13] <= 172.9175033569336) {
                    if (input[21] <= 0.48499999940395355) {
                        if (input[9] <= 0.11999999731779099) {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[27] <= 1.175950050354004) {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[8] <= 8.639949798583984) {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[8] <= 8.906399726867676) {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (input[1] <= 135.5) {
            if (input[22] <= 4.062049865722656) {
                if (input[4] <= 0.875) {
                    if (input[21] <= 0.9114000201225281) {
                        if (input[29] <= 33.5) {
                            if (input[10] <= -1.5) {
                                if (input[24] <= 1.1950000524520874) {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.7692307692307693, 0.23076923076923078}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[14] <= 886.9299926757812) {
                                    memcpy(var25, (double[]){0.9924242424242424, 0.007575757575757576}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[3] <= 2.415000081062317) {
                                if (input[10] <= 92.0) {
                                    memcpy(var25, (double[]){0.9666666666666667, 0.03333333333333333}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 84.5) {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.13274336283185842, 0.8672566371681416}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[10] <= -114.0) {
                            if (input[18] <= 4.03000009059906) {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[14] <= 546.1915893554688) {
                                if (input[14] <= 290.19085693359375) {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.8, 0.2}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[1] <= 103.5) {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[9] <= 12.418099880218506) {
                        if (input[8] <= 5.514650106430054) {
                            if (input[27] <= 1.30349999666214) {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[9] <= 5.9362499713897705) {
                                if (input[8] <= 7.5632500648498535) {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[1] <= 132.5) {
                    if (input[22] <= 5.075000047683716) {
                        if (input[1] <= 116.0) {
                            if (input[1] <= 113.0) {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[21] <= 0.5) {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[7] <= 2.5) {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[7] <= 10.5) {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[1] <= 130.5) {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[22] <= 5.7149999141693115) {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[23] <= 2.746649980545044) {
                            if (input[6] <= 134.5) {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[22] <= 5.7149999141693115) {
                if (input[6] <= 1133.5) {
                    if (input[19] <= 0.5799999833106995) {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[19] <= 1.1693499684333801) {
                            if (input[8] <= 0.5) {
                                if (input[22] <= 3.6050000190734863) {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[23] <= 2.395750045776367) {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[9] <= 0.4599999934434891) {
                                    memcpy(var25, (double[]){0.17073170731707318, 0.8292682926829268}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){0.04924242424242424, 0.9507575757575758}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[14] <= 780.5549926757812) {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[24] <= 1.340000033378601) {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[13] <= 647.1455078125) {
                    if (input[26] <= 9.875249862670898) {
                        if (input[6] <= 8.5) {
                            if (input[0] <= 96.5) {
                                memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[23] <= 2.7457000017166138) {
                            if (input[7] <= 3.5) {
                                memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[3] <= 2.7213000059127808) {
                                    memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 13.0) {
                        memcpy(var25, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var25, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var6, var25, 2, var5);
    double var26[2];
    if (input[1] <= 135.5) {
        if (input[3] <= 2.295849919319153) {
            if (input[16] <= 0.5) {
                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[3] <= 1.7675999999046326) {
                    if (input[1] <= 130.5) {
                        if (input[29] <= 65.5) {
                            memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[15] <= 979.7831115722656) {
                                if (input[0] <= 56.5) {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.8181818181818182, 0.18181818181818182}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[1] <= 133.5) {
                            if (input[27] <= 0.9223999977111816) {
                                if (input[0] <= 97.5) {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[25] <= 1.4440999627113342) {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[16] <= 0.5) {
                if (input[10] <= 47.5) {
                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[1] <= 127.5) {
                        memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[22] <= 5.4688498973846436) {
                            memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[23] <= 2.39710009098053) {
                    if (input[29] <= 42.5) {
                        if (input[14] <= 582.5750122070312) {
                            if (input[15] <= 908.2492980957031) {
                                if (input[1] <= 91.0) {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[13] <= 65.90499877929688) {
                                if (input[1] <= 93.0) {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[3] <= 6.440000057220459) {
                                    memcpy(var26, (double[]){0.9393939393939394, 0.06060606060606061}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.5789473684210527, 0.42105263157894735}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[0] <= 45.5) {
                            if (input[6] <= 382.5) {
                                if (input[10] <= 74.0) {
                                    memcpy(var26, (double[]){0.9939393939393939, 0.006060606060606061}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[19] <= 1.050000011920929) {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[0] <= 59.5) {
                                if (input[3] <= 6.555000066757202) {
                                    memcpy(var26, (double[]){0.23404255319148937, 0.7659574468085106}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.7333333333333333, 0.26666666666666666}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[23] <= 2.662950038909912) {
                        if (input[26] <= 9.748400211334229) {
                            if (input[1] <= 121.5) {
                                if (input[1] <= 113.0) {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.7777777777777778, 0.2222222222222222}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[10] <= 90.5) {
                                if (input[26] <= 9.760000228881836) {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.9629629629629629, 0.037037037037037035}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[1] <= 149.5) {
            if (input[13] <= 42.385000228881836) {
                if (input[22] <= 6.090000152587891) {
                    if (input[17] <= 0.7700000107288361) {
                        memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[23] <= 2.6895999908447266) {
                    if (input[22] <= 5.349999904632568) {
                        memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[19] <= 1.3250000476837158) {
                            if (input[27] <= 1.2450000047683716) {
                                if (input[8] <= 5.325000047683716) {
                                    memcpy(var26, (double[]){0.125, 0.875}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[27] <= 1.1551499962806702) {
                                if (input[9] <= 1.3149999976158142) {
                                    memcpy(var26, (double[]){0.07692307692307693, 0.9230769230769231}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[14] <= 163.5323944091797) {
                        if (input[24] <= 0.7582499980926514) {
                            if (input[4] <= 3.5831499695777893) {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[15] <= 132.22855377197266) {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[4] <= 0.20714999735355377) {
                            if (input[13] <= 290.32801055908203) {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[27] <= 1.0184499621391296) {
                                if (input[7] <= 9.0) {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            }
        } else {
            if (input[20] <= 0.5253000259399414) {
                if (input[1] <= 167.5) {
                    if (input[23] <= 2.3996500968933105) {
                        if (input[27] <= 1.4149999618530273) {
                            memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 2.807900071144104) {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[25] <= 1.3849999904632568) {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[10] <= -8.0) {
                            if (input[13] <= 341.0) {
                                if (input[27] <= 1.2950000166893005) {
                                    memcpy(var26, (double[]){0.1, 0.9}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[27] <= 1.1485499739646912) {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[15] <= 264.95499420166016) {
                                if (input[27] <= 1.2999999523162842) {
                                    memcpy(var26, (double[]){0.05263157894736842, 0.9473684210526315}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.9166666666666666, 0.08333333333333333}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[17] <= 0.7307500243186951) {
                        if (input[26] <= 4.529699802398682) {
                            if (input[19] <= 0.7824000120162964) {
                                memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[29] <= 110.5) {
                                if (input[4] <= 9.139999866485596) {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.6, 0.4}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[10] <= 118.5) {
                                    memcpy(var26, (double[]){0.2727272727272727, 0.7272727272727273}, 2 * sizeof(double));
                                } else {
                                    memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[27] <= 1.9100000262260437) {
                            memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[2] <= 4.5) {
                    if (input[22] <= 4.664999961853027) {
                        if (input[4] <= 15.934849739074707) {
                            memcpy(var26, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var26, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var5, var26, 2, var4);
    double var27[2];
    if (input[1] <= 135.5) {
        if (input[27] <= 0.9291000068187714) {
            if (input[16] <= 0.5) {
                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
            } else {
                if (input[10] <= 131.5) {
                    if (input[23] <= 2.662950038909912) {
                        if (input[10] <= 84.0) {
                            if (input[0] <= 86.5) {
                                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[24] <= 1.309850037097931) {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[27] <= 0.8883500099182129) {
                                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[23] <= 2.8128000497817993) {
                            if (input[27] <= 0.82710000872612) {
                                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[17] <= 0.8481000065803528) {
                                    memcpy(var27, (double[]){0.9636363636363636, 0.03636363636363636}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                }
            }
        } else {
            if (input[1] <= 84.5) {
                if (input[16] <= 0.5) {
                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[7] <= 9.5) {
                        memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[22] <= 2.175700068473816) {
                            if (input[1] <= 44.5) {
                                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[22] <= 3.9627000093460083) {
                    if (input[7] <= 11.5) {
                        if (input[26] <= 8.35890007019043) {
                            if (input[4] <= 0.4150000065565109) {
                                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[17] <= 0.6911500096321106) {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){0.1875, 0.8125}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[26] <= 8.468550205230713) {
                                if (input[9] <= 9.035099983215332) {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[14] <= 920.9349975585938) {
                                    memcpy(var27, (double[]){0.03225806451612903, 0.967741935483871}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[9] <= 12.813849925994873) {
                            memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[23] <= 2.6450001001358032) {
                        if (input[27] <= 1.1006500124931335) {
                            if (input[12] <= 0.5) {
                                if (input[4] <= 0.8276499807834625) {
                                    memcpy(var27, (double[]){0.85, 0.15}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[18] <= 4.275000095367432) {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){0.09090909090909091, 0.9090909090909091}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[27] <= 1.8350000381469727) {
            if (input[22] <= 5.7149999141693115) {
                if (input[19] <= 1.437150001525879) {
                    if (input[18] <= 37.135650634765625) {
                        if (input[23] <= 2.3996500968933105) {
                            if (input[26] <= 5.355000019073486) {
                                if (input[19] <= 0.9900000095367432) {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[1] <= 158.5) {
                                if (input[4] <= 0.3449999988079071) {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){0.1702127659574468, 0.8297872340425532}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[7] <= 10.5) {
                                    memcpy(var27, (double[]){0.8076923076923077, 0.19230769230769232}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[4] <= 11.883199691772461) {
                            memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[27] <= 1.0250999927520752) {
                    if (input[21] <= 0.8950000107288361) {
                        if (input[14] <= 75.8765983581543) {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[29] <= 96.0) {
                                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[28] <= 7.0) {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[4] <= 0.21289999783039093) {
                            memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[14] <= 220.72205352783203) {
                        if (input[14] <= 33.28814888000488) {
                            memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 95.5) {
                            if (input[29] <= 120.5) {
                                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[2] <= 3.5) {
                                    memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[8] <= 7.640249729156494) {
                                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    }
                }
            }
        } else {
            if (input[17] <= 0.5769500136375427) {
                memcpy(var27, (double[]){0.0, 1.0}, 2 * sizeof(double));
            } else {
                memcpy(var27, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        }
    }
    add_vectors(var4, var27, 2, var3);
    double var28[2];
    if (input[3] <= 2.298949956893921) {
        if (input[1] <= 136.5) {
            if (input[1] <= 131.5) {
                if (input[27] <= 0.9639999866485596) {
                    if (input[0] <= 96.5) {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[26] <= 10.419849872589111) {
                            if (input[8] <= 7.09119987487793) {
                                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 88.0) {
                        if (input[4] <= 2.937649965286255) {
                            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[9] <= 7.825399875640869) {
                                if (input[4] <= 3.451650023460388) {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[3] <= 1.8423500061035156) {
                            memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[26] <= 9.927350044250488) {
                    if (input[24] <= 1.0683000087738037) {
                        if (input[23] <= 2.588849902153015) {
                            memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[20] <= -0.3341500014066696) {
                        if (input[3] <= 1.7275999784469604) {
                            memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[1] <= 139.5) {
                if (input[3] <= 1.804800033569336) {
                    if (input[15] <= 1182.4126586914062) {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[3] <= 2.0962499380111694) {
                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[22] <= 4.669999957084656) {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[0] <= 87.5) {
            if (input[10] <= 100.5) {
                if (input[22] <= 4.379999876022339) {
                    if (input[0] <= 53.0) {
                        if (input[10] <= -84.5) {
                            if (input[1] <= 80.0) {
                                memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[22] <= 2.6679999828338623) {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[1] <= 83.0) {
                                memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[26] <= 7.450099945068359) {
                                    memcpy(var28, (double[]){0.2, 0.8}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[7] <= 11.5) {
                            if (input[18] <= 20.415000438690186) {
                                if (input[22] <= 3.25) {
                                    memcpy(var28, (double[]){0.008333333333333333, 0.9916666666666667}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){0.20454545454545456, 0.7954545454545454}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[8] <= 6.4967498779296875) {
                                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    }
                } else {
                    if (input[1] <= 122.5) {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 61.5) {
                    if (input[10] <= 107.0) {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[23] <= 2.486400008201599) {
                        memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[16] <= 0.5) {
                if (input[19] <= 1.4444500207901) {
                    if (input[19] <= 1.1149500012397766) {
                        if (input[2] <= 4.5) {
                            memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            if (input[22] <= 0.7129999995231628) {
                                if (input[9] <= 9.62000036239624) {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[26] <= 5.866850137710571) {
                            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[29] <= 115.5) {
                                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[10] <= -42.0) {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[19] <= 1.3950499892234802) {
                    if (input[26] <= 8.703499794006348) {
                        if (input[6] <= 0.5) {
                            if (input[3] <= 6.0) {
                                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[25] <= 1.215000033378601) {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[1] <= 163.5) {
                                if (input[22] <= 4.740000009536743) {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){0.7368421052631579, 0.2631578947368421}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[4] <= 0.24499999731779099) {
                            if (input[26] <= 9.74025011062622) {
                                memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[23] <= 2.6549999713897705) {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[23] <= 2.5999999046325684) {
                                if (input[27] <= 1.2450000047683716) {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[17] <= 0.8481000065803528) {
                                    memcpy(var28, (double[]){0.625, 0.375}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){0.975, 0.025}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    if (input[27] <= 0.8671500086784363) {
                        memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[22] <= 5.870000123977661) {
                            if (input[20] <= 0.054999999701976776) {
                                if (input[20] <= -0.2499999925494194) {
                                    memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){0.047619047619047616, 0.9523809523809523}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[11] <= 12.0) {
                                    memcpy(var28, (double[]){0.8, 0.2}, 2 * sizeof(double));
                                } else {
                                    memcpy(var28, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var28, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var3, var28, 2, var2);
    double var29[2];
    if (input[1] <= 135.5) {
        if (input[29] <= 32.5) {
            if (input[14] <= 531.7300109863281) {
                if (input[2] <= 2.5) {
                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[15] <= 276.4971618652344) {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[6] <= 76.5) {
                            if (input[20] <= -0.2722500041127205) {
                                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[0] <= 52.0) {
                                memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                if (input[1] <= 103.5) {
                                    memcpy(var29, (double[]){0.5, 0.5}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.9545454545454546, 0.045454545454545456}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                }
            } else {
                if (input[19] <= 1.2549999952316284) {
                    if (input[10] <= -18.5) {
                        if (input[14] <= 739.5920715332031) {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 148.69499969482422) {
                            if (input[18] <= 1.2000000476837158) {
                                if (input[13] <= 134.95999908447266) {
                                    memcpy(var29, (double[]){0.625, 0.375}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[29] <= 27.5) {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        if (input[23] <= 2.625) {
                            if (input[21] <= 0.35999999940395355) {
                                memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[1] <= 114.5) {
                if (input[19] <= 1.2754499912261963) {
                    if (input[18] <= 3.8399999141693115) {
                        if (input[0] <= 51.0) {
                            if (input[14] <= 881.0449829101562) {
                                if (input[0] <= 43.5) {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.8823529411764706, 0.11764705882352941}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[14] <= 885.8672485351562) {
                                    memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.96875, 0.03125}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[19] <= 1.0699999928474426) {
                                if (input[8] <= 4.894999861717224) {
                                    memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[0] <= 55.5) {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.09523809523809523, 0.9047619047619048}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        if (input[18] <= 5.2804999351501465) {
                            if (input[1] <= 100.0) {
                                if (input[22] <= 2.5730000734329224) {
                                    memcpy(var29, (double[]){0.16666666666666666, 0.8333333333333334}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[17] <= 0.6911500096321106) {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.08695652173913043, 0.9130434782608695}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            if (input[20] <= -0.6899999976158142) {
                                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[14] <= 357.88490295410156) {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.875, 0.125}, 2 * sizeof(double));
                                }
                            }
                        }
                    }
                } else {
                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                if (input[4] <= 0.22190000116825104) {
                    if (input[11] <= 29.0) {
                        if (input[26] <= 9.865000247955322) {
                            if (input[0] <= 82.5) {
                                memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            } else {
                                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            }
                        } else {
                            if (input[19] <= 1.485450029373169) {
                                if (input[6] <= 151.0) {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.8421052631578947, 0.15789473684210525}, 2 * sizeof(double));
                                }
                            } else {
                                memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                            }
                        }
                    } else {
                        if (input[15] <= 396.44635009765625) {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[22] <= 5.490449905395508) {
                        if (input[23] <= 1.7893499732017517) {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            if (input[27] <= 1.0649999976158142) {
                                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[7] <= 6.5) {
                                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                }
                            }
                        }
                    } else {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[22] <= 5.865000009536743) {
            if (input[20] <= 0.4237000048160553) {
                if (input[2] <= 4.5) {
                    if (input[1] <= 167.5) {
                        if (input[19] <= 1.490149974822998) {
                            if (input[2] <= 2.5) {
                                memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                            } else {
                                if (input[22] <= 5.439100027084351) {
                                    memcpy(var29, (double[]){0.06521739130434782, 0.9347826086956522}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.6296296296296297, 0.37037037037037035}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        if (input[1] <= 346.0) {
                            if (input[5] <= 99.5) {
                                if (input[27] <= 1.7899999618530273) {
                                    memcpy(var29, (double[]){0.00031710797526557794, 0.9996828920247344}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.2, 0.8}, 2 * sizeof(double));
                                }
                            } else {
                                if (input[8] <= 7.902250051498413) {
                                    memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                                } else {
                                    memcpy(var29, (double[]){0.75, 0.25}, 2 * sizeof(double));
                                }
                            }
                        } else {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                } else {
                    if (input[25] <= 1.383899986743927) {
                        memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        if (input[4] <= 6.920000076293945) {
                            memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[4] <= 6.079999923706055) {
                    memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                } else {
                    if (input[3] <= 3.906749963760376) {
                        memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[2] <= 2.5) {
                if (input[13] <= 306.82801055908203) {
                    memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                } else {
                    if (input[26] <= 9.881199836730957) {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            } else {
                if (input[3] <= 2.5879499912261963) {
                    if (input[23] <= 2.732100009918213) {
                        if (input[4] <= 0.557449996471405) {
                            memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    if (input[10] <= 141.0) {
                        memcpy(var29, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    } else {
                        memcpy(var29, (double[]){0.0, 1.0}, 2 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var2, var29, 2, var1);
    mul_vector_number(var1, 0.06666666666666667, 2, var0);
    memcpy(output, var0, 2 * sizeof(double));
}

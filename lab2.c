#include <stdio.h>
#include <string.h>

void unicodeToUtf8(unsigned long code, unsigned char res[4], int *count) {
    *count = 0;
    if (code <= 0x7F || (code >= 0xDC00 && code <= 0xDCFF)) {
        res[0] = code;
        *count = 1;
    } else if (code <= 0x07FF) {
        res[0] = ((code & 0x7C0) >> 6) | 0xC0;
        res[1] = (code & 0x3F) | 0x80;
        *count = 2;
    } else if (code <= 0xFFFF) {
        res[0] = ((code & 0xF000) >> 12) | 0xE0;
        res[1] = ((code & 0xFC0) >> 6) | 0x80;
        res[2] = (code & 0x3F) | 0x80;
        *count = 3;
    } else if (code <= 0x10FFFF) {
        res[0] = ((code & 0x1C0000) >> 18) | 0xF0;
        res[1] = ((code & 0x3F000) >> 12) | 0x80;
        res[2] = ((code & 0xFC0) >> 6) | 0x80;
        res[3] = (code & 0x3F) | 0x80;
        *count = 4;
    }
}

void unicodeToUtf16BE(unsigned long code, unsigned char res[4], int *count) {
    *count = 0;
    if (code <= 0xFFFF) {
        res[0] = (code & 0xFF00) >> 8;
        res[1] = code & 0x00FF;
        *count = 2;
    } else if (code <= 0x10FFFF) {
        code -= 0x10000;
        unsigned long w1 = (code & 0xFFC00) >> 10 | 0xD800;
        unsigned long w2 = (code & 0x3FF) | 0xDC00;
        res[0] = (w1 & 0xFF00) >> 8;
        res[1] = (w1 & 0x00FF);
        res[2] = (w2 & 0xFF00) >> 8;
        res[3] = (w2 & 0x00FF);
        *count = 4;
    }
}

void unicodeToUtf16LE(unsigned long code, unsigned char res[4], int *count) {
    *count = 0;
    if (code <= 0xFFFF) {
        res[1] = (code & 0xFF00) >> 8;
        res[0] = code & 0x00FF;
        *count = 2;
    } else if (code <= 0x10FFFF) {
        code -= 0x10000;
        unsigned long w1 = (code & 0xFFC00) >> 10 | 0xD800;
        unsigned long w2 = (code & 0x3FF) | 0xDC00;
        res[1] = (w1 & 0xFF00) >> 8;
        res[0] = (w1 & 0x00FF);
        res[3] = (w2 & 0xFF00) >> 8;
        res[2] = (w2 & 0x00FF);
        *count = 4;
    }
}

void unicodeToUtf32BE(unsigned long code, unsigned char res[4], int *count) {
    *count = 0;
    if (code <= 0xFF) {
        res[0] = 0;
        res[1] = 0;
        res[2] = 0;
        res[3] = code;
        *count = 4;
    } else if (code <= 0xFFFF) {
        res[0] = 0;
        res[1] = 0;
        res[2] = (code & 0xFF00) >> 8;
        res[3] = code & 0xFF;
        *count = 4;
    } else if (code <= 0xFFFFFF) {
        res[0] = 0;
        res[1] = (code & 0xFF0000) >> 16;
        res[2] = (code & 0xFF00) >> 8;
        res[3] = (code & 0xFF);
        *count = 4;
    } else {
        res[0] = (code & 0xFF000000) >> 24;
        res[1] = (code & 0xFF0000) >> 16;
        res[2] = (code & 0xFF00) >> 8;
        res[3] = code & 0xFF;
        *count = 4;
    }
}

void unicodeToUtf32LE(unsigned long code, unsigned char res[4], int *count) {
    *count = 0;
    if (code <= 0xFF) {
        res[0] = code;
        res[1] = 0;
        res[2] = 0;
        res[3] = 0;
        *count = 4;
    } else if (code <= 0xFFFF) {
        res[3] = 0;
        res[2] = 0;
        res[1] = (code & 0xFF00) >> 8;
        res[0] = code & 0xFF;
        *count = 4;
    } else if (code <= 0xFFFFFF) {
        res[3] = 0;
        res[2] = (code & 0xFF0000) >> 16;
        res[1] = (code & 0xFF00) >> 8;
        res[0] = (code & 0xFF);
        *count = 4;
    } else {
        res[3] = (code & 0xFF000000) >> 24;
        res[2] = (code & 0xFF0000) >> 16;
        res[1] = (code & 0xFF00) >> 8;
        res[0] = code & 0xFF;
        *count = 4;
    }
}

unsigned long utf8toUnicode(unsigned const char res[4], int count) {
    unsigned long code;
    switch (count) {
        case 1:
            code = res[0];
            break;
        case 2:
            code = ((res[0] - 0xC0) << 6) + (res[1] - 0x80);
            break;
        case 3:
            code = ((res[0] - 0xE0) << 12) + ((res[1] - 0x80) << 6) + (res[2] - 0x80);
            break;
        case 4:
            code = ((res[0] - 0xF0) << 18) + ((res[1] - 0x80) << 12) + ((res[2] - 0x80) << 6) + (res[3] - 0x80);
            break;
        default:
            code = 0xFFFFFFFF;
            break;
    }
    return code;
}

unsigned long utf16BEtoUnicode(unsigned const char res[4], int count) {
    unsigned long code;
    switch (count) {
        case 2:
            code = (res[0] << 8) + res[1];
            break;
        case 4:
            code = ((((res[0] << 8) + res[1]) - 0xD800) << 10) + (((res[2] << 8) + res[3]) - 0xDC00) + 0x10000;
            break;
        default:
            code = -1;
            break;
    }
    return code;
}

unsigned long utf16LEtoUnicode(unsigned const char res[4], int count) {
    unsigned long code;
    switch (count) {
        case 2:
            code = (res[1] << 8) + res[0];
            break;
        case 4:
            code = ((((res[1] << 8) + res[0]) - 0xD800) << 10) + (((res[3] << 8) + res[2]) - 0xDC00) + 0x10000;
            break;
        default:
            code = -1;
            break;
    }
    return code;
}

unsigned long utf32BEtoUnicode(unsigned const char res[4], int count) {
    if (count == 4) {
        return (res[0] << 24) + (res[1] << 16) + (res[2] << 8) + (res[3]);
    } else {
        return 0;
    }
}

unsigned long utf32LEtoUnicode(unsigned const char res[4], int count) {
    if (count == 4) {
        return (res[3] << 24) + (res[2] << 16) + (res[1] << 8) + (res[0]);
    } else {
        return 0;
    }
}

void utf8ToOutput(FILE *in, FILE *out, void (*unicodeToUtf)(unsigned long, unsigned char *, int *)) {
    unsigned char c[4];
    int count;
    unsigned long code;
    while (1) {
        fread(&c[0], sizeof(unsigned char), 1, in);
        if (feof(in)) {
            break;
        }
        if (c[0] < 0x80) {
            count = 1;
            code = utf8toUnicode(c, count);
        } else if (c[0] >= 0xC2 && c[0] <= 0xDF) {
            fread(&c[1], sizeof(unsigned char), 1, in);
            if (!feof(in)) {
                if (c[1] >= 0x80 && c[1] <= 0xBF) {
                    count = 2;
                    code = utf8toUnicode(c, count);
                } else {
                    count = -1;
                }
            }
        } else if (c[0] >= 0xE0 && c[0] <= 0xEF) {
            fread(&c[1], sizeof(unsigned char), 1, in);
            if (!feof(in)) {
                fread(&c[2], sizeof(unsigned char), 1, in);
                if (!feof(in)) {
                    if (c[1] >= 0x80 && c[1] <= 0xBF && c[2] >= 0x80 && c[2] <= 0xBF
                        && !(c[0] == 0xE0 && c[1] < 0xA0)) {
                        count = 3;
                        code = utf8toUnicode(c, count);
                    } else {
                        count = -2;
                    }
                } else {
                    count = -1;
                }
            }
        } else if (c[0] >= 0xF0 && c[0] <= 0xF4) {
            fread(&c[1], sizeof(unsigned char), 1, in);
            if (!feof(in)) {
                fread(&c[2], sizeof(unsigned char), 1, in);
                if (!feof(in)) {
                    fread(&c[3], sizeof(unsigned char), 1, in);
                    if (!feof(in)) {
                        if (c[1] >= 0x80 && c[1] <= 0xBF && c[2] >= 0x80 && c[2] <= 0xBF && c[3] >= 0x80 &&
                            c[3] <= 0xBF && !(c[0] == 0xF0 && c[1] < 0x90 || c[0] == 0xF4 && c[1] >= 0x90)) {
                            count = 4;
                            code = utf8toUnicode(c, count);
                        } else {
                            count = -3;
                        }
                    } else {
                        count = -2;
                    }
                } else {
                    count = -1;
                }
            }
        }
        if (count <= 0) {
            int k = -count;
            for (int i = 0; i < k; ++i) {
                code = 0xDC00 + c[i];
                unicodeToUtf(code, c, &count);
                fwrite(&c, sizeof(unsigned char), count, out);
            }
            code = 0xDC00 + c[k];
        }
        unicodeToUtf(code, c, &count);
        fwrite(&c, sizeof(unsigned char), count, out);
    }
}

void utf16LEToOutput(FILE *in, FILE *out, void (*unicodeToUtf)(unsigned long, unsigned char *, int *)) {
    unsigned char c[4];
    int count;
    unsigned long code;
    while (1) {
        fread(&c, sizeof(unsigned char), 2, in);
        if (!feof(in)) {
            if (c[1] >= 0xD8 && c[1] <= 0xDB) {
                fread(&c[2], sizeof(unsigned char), 2, in);
                code = utf16LEtoUnicode(c, 4);
            } else {
                code = utf16LEtoUnicode(c, 2);
            }
            unicodeToUtf(code, c, &count);
            fwrite(&c, sizeof(unsigned char), count, out);
        } else {
            break;
        }
    }
}

void utf16BEToOutput(FILE *in, FILE *out, void (*unicodeToUtf)(unsigned long, unsigned char *, int *)) {
    unsigned char c[4];
    int count;
    unsigned long code;
    while (1) {
        fread(&c, sizeof(unsigned char), 2, in);
        if (!feof(in)) {
            if (c[0] >= 0xD8 && c[0] <= 0xDB) {
                fread(&c[2], sizeof(unsigned char), 2, in);
                code = utf16BEtoUnicode(c, 4);
            } else {
                code = utf16BEtoUnicode(c, 2);
            }
            unicodeToUtf(code, c, &count);
            fwrite(&c, sizeof(unsigned char), count, out);
        } else {
            break;
        }
    }
}

void utf32LEToOutput(FILE *in, FILE *out, int *InputEncoding,
                     void (*unicodeToUtf)(unsigned long, unsigned char *, int *)) {
    unsigned char c[4];
    int count;
    unsigned long code;
    while (1) {
        fread(&c, sizeof(unsigned char), 4, in);
        if (!feof(in)) {
            code = utf32LEtoUnicode(c, 4);
            if (code > 0x10FFFF) {
                *InputEncoding = 2;
                fseek(in, 2, SEEK_SET);
                break;
            }
            unicodeToUtf(code, c, &count);
            fwrite(&c, sizeof(unsigned char), count, out);
        } else {
            *InputEncoding = -1;
            break;
        }
    }
}

void utf32BEToOutput(FILE *in, FILE *out, void (*unicodeToUtf)(unsigned long, unsigned char *, int *)) {
    unsigned char c[4];
    int count;
    unsigned long code;
    while (1) {
        fread(&c, sizeof(unsigned char), 4, in);
        if (!feof(in)) {
            code = utf32BEtoUnicode(c, 4);
            unicodeToUtf(code, c, &count);
            fwrite(&c, sizeof(unsigned char), count, out);
        } else {
            break;
        }
    }
}

void writeBOM(FILE *out, int OutputEncoding) {
    unsigned char c[4];
    int count = 0;
    switch (OutputEncoding) {
        case 1:
            unicodeToUtf8(0xFEFF, c, &count);
            break;
        case 2:
            unicodeToUtf16LE(0xFEFF, c, &count);
            break;
        case 3:
            unicodeToUtf16BE(0xFEFF, c, &count);
            break;
        case 4:
            unicodeToUtf32LE(0xFEFF, c, &count);
            break;
        case 5:
            unicodeToUtf32BE(0xFEFF, c, &count);
        default:
            break;
    }
    fwrite(&c, sizeof(unsigned char), count, out);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("%s", "Usage: name of programme, input file, output file, output encoding");
        return 1;
    }
    FILE *in = fopen(argv[1], "rb");
    if (in == NULL) {
        printf("%s", "InputFile open error");
        return 1;
    }
    int OutputEncoding = argv[3][0] - '0';
    if (OutputEncoding < 0 || OutputEncoding > 5) {
        fclose(in);
        printf("%s", "Output encoding argument error");
        return 1;
    }

    unsigned char BOM[4];
    fread(BOM, sizeof(unsigned char), 2, in);
    int InputEncoding = -1;

    if (!feof(in)) {
        if (BOM[0] == 0xEF && BOM[1] == 0xBB) {
            fread(&BOM[2], sizeof(unsigned char), 1, in);
            if (!feof(in)) {
                if (BOM[2] == 0xBF) {
                    //utf8bom input
                    InputEncoding = 1;
                } else {
                    fseek(in, 0, SEEK_SET);
                }
            } else {
                fseek(in, 0, SEEK_SET);
            }
        } else if (BOM[0] == 0xFF && BOM[1] == 0xFE) {
            //utf32le or 16le
            fread(&BOM[2], sizeof(unsigned char), 2, in);
            if (!feof(in)) {
                if (BOM[2] == 0x00 && BOM[3] == 0x00) {
                    //utf32le or 16le
                    InputEncoding = 4;
                } else {
                    //utf16le
                    fseek(in, -2, SEEK_CUR);
                    InputEncoding = 2;
                }
            } else {
                //utf16le
                InputEncoding = 2;
            }
        } else if (BOM[0] == 0xFE && BOM[1] == 0xFF) {
            //utf16be
            InputEncoding = 3;
        } else if (BOM[0] == 0x00 && BOM[1] == 0x00) {
            fread(&BOM[2], sizeof(unsigned char), 2, in);
            if (!feof(in)) {
                if (BOM[2] == 0xFE && BOM[3] == 0xFF) {
                    //utf32be
                    InputEncoding = 5;
                }
            }
        }
    }
    if (InputEncoding == -1) {
        //utf8
        fseek(in, 0, SEEK_SET);
        InputEncoding = 0;
    }
    while (InputEncoding != -1) {
        FILE *out = fopen(argv[2], "wb");
        if (out == NULL) {
            fclose(out);
            printf("%s", "OutputFile open error");
            return 1;
        }
        writeBOM(out, OutputEncoding);
        switch (InputEncoding) {
            case 0:
            case 1:
                switch (OutputEncoding) {
                    case 0:
                    case 1:
                    default:
                        while (1) {
                            unsigned char c;
                            fread(&c, sizeof(c), 1, in);
                            if (feof(in)) {
                                break;
                            }
                            fwrite(&c, sizeof(c), 1, out);
                        }
                        break;
                    case 2:
                        utf8ToOutput(in, out, unicodeToUtf16LE);
                        break;
                    case 3:
                        utf8ToOutput(in, out, unicodeToUtf16BE);
                        break;
                    case 4:
                        utf8ToOutput(in, out, unicodeToUtf32LE);
                        break;
                    case 5:
                        utf8ToOutput(in, out, unicodeToUtf32BE);
                        break;
                }
                InputEncoding = -1;
                break;
            case 2:
                switch (OutputEncoding) {
                    case 0:
                    case 1:
                    default:
                        utf16LEToOutput(in, out, unicodeToUtf8);
                        break;
                    case 2:
                        utf16LEToOutput(in, out, unicodeToUtf16LE);
                        break;
                    case 3:
                        utf16LEToOutput(in, out, unicodeToUtf16BE);
                        break;
                    case 4:
                        utf16LEToOutput(in, out, unicodeToUtf32LE);
                        break;
                    case 5:
                        utf16LEToOutput(in, out, unicodeToUtf32BE);
                        break;
                }
                InputEncoding = -1;
                break;
            case 3:
                switch (OutputEncoding) {
                    case 0:
                    case 1:
                    default:
                        utf16BEToOutput(in, out, unicodeToUtf8);
                        break;
                    case 2:
                        utf16BEToOutput(in, out, unicodeToUtf16LE);
                        break;
                    case 3:
                        utf16BEToOutput(in, out, unicodeToUtf16BE);
                        break;
                    case 4:
                        utf16BEToOutput(in, out, unicodeToUtf32LE);
                        break;
                    case 5:
                        utf16BEToOutput(in, out, unicodeToUtf32BE);
                        break;
                }
                InputEncoding = -1;
                break;
            case 4:
                switch (OutputEncoding) {
                    case 0:
                    case 1:
                    default:
                        utf32LEToOutput(in, out, &InputEncoding, unicodeToUtf8);
                        break;
                    case 2:
                        utf32LEToOutput(in, out, &InputEncoding, unicodeToUtf16LE);
                        break;
                    case 3:
                        utf32LEToOutput(in, out, &InputEncoding, unicodeToUtf16BE);
                        break;
                    case 4:
                        utf32LEToOutput(in, out, &InputEncoding, unicodeToUtf32LE);
                        break;
                    case 5:
                        utf32LEToOutput(in, out, &InputEncoding, unicodeToUtf32BE);
                        break;
                }
                break;
            case 5:
                switch (OutputEncoding) {
                    case 1:
                    case 0:
                    default:
                        utf32BEToOutput(in, out, unicodeToUtf8);
                        break;
                    case 2:
                        utf32BEToOutput(in, out, unicodeToUtf16LE);
                        break;
                    case 3:
                        utf32BEToOutput(in, out, unicodeToUtf16BE);
                        break;
                    case 4:
                        utf32BEToOutput(in, out, unicodeToUtf32LE);
                        break;
                    case 5:
                        utf32BEToOutput(in, out, unicodeToUtf32BE);
                        break;
                }
                InputEncoding = -1;
                break;
        }
        fclose(out);
    }
    fclose(in);
    return 0;
}
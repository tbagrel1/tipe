/* find the suffix array SA of T[0..n-1] in {0..255}^n */
static sais_index_type
sais_main(
    const sais_char_type *T, sais_index_type *SA, sais_index_type fs,
    sais_index_type n, sais_index_type k, sais_bool_type isbwt) {
    sais_index_type *C, *B, *D, *RA, *b;
    sais_index_type i, j, m, p, q, t, name, pidx = 0, newfs;
    unsigned int flags;
    sais_char_type c0, c1;

    assert((T != NULL) && (SA != NULL));
    assert((0 <= fs) && (0 < n) && (1 <= k));


    if ((C = SAIS_MYMALLOC(k, sais_index_type)) == NULL) {
        return -2;
    }
    if (k <= fs) {
        B = SA + (n + fs - k);
        flags = 1;
    } else {
        if ((B = SAIS_MYMALLOC(k, sais_index_type)) == NULL) {
            SAIS_MYFREE(C, k, sais_index_type);
            return -2;
        }
        flags = 3;
    }

    if ((n <= SAIS_LMSSORT2_LIMIT) && (2 <= (n / k))) {
        if (flags & 1) {
            flags |= ((k * 2) <= (fs - k)) ? 32 : 16;
        } else if ((flags == 0) && ((k * 2) <= (fs - k * 2))) {
            flags |= 32;
        }
    }

    /* stage 1: reduce the problem by at least 1/2
       sort all the LMS-substrings */
    getCounts(T, C, n, k);
    getBuckets(C, B, k, 1); /* find ends of buckets */
    for (
        i = 0; i < n; ++i) {
        SA[i] = 0;
    }
    b = &t;
    i = n - 1;
    j = n;
    m = 0;
    c0 = chr(n - 1);
    do {
        c1 = c0;
    } while ((0 <= --i) && ((c0 = chr(i)) >= c1));
    for (
        ; 0 <= i;) {
        do {
            c1 = c0;
        } while ((0 <= --i) && ((c0 = chr(i)) <= c1));
        if (0 <= i) {
            *b = j;
            b = SA + --B[c1];
            j = i;
            ++m;
            do {
                c1 = c0;
            } while ((0 <= --i) && ((c0 = chr(i)) >= c1));
        }
    }

    if (1 < m) {
        if (flags & (16 | 32)) {
            if (flags & 16) {
                if ((D = SAIS_MYMALLOC(k * 2, sais_index_type)) == NULL) {
                    if (flags & (1 | 4)) {
                        SAIS_MYFREE(C, k, sais_index_type);
                    }
                    if (flags & 2) {
                        SAIS_MYFREE(B, k, sais_index_type);
                    }
                    return -2;
                }
            } else {
                D = B - k * 2;
            }
            assert((j + 1) < n);
            ++B[chr(j + 1)];
            for (
                i = 0, j = 0; i < k; ++i) {
                j += C[i];
                if (B[i] != j) {
                    assert(SA[B[i]] != 0);
                    SA[B[i]] += n;
                }
                D[i] = D[i + k] = 0;
            }
            LMSsort2(T, SA, C, B, D, n, k);
            name = LMSpostproc2(SA, n, m);
            if (flags & 16) {
                SAIS_MYFREE(D, k * 2, sais_index_type);
            }
        } else {
            LMSsort1(T, SA, C, B, n, k);
            name = LMSpostproc1(T, SA, n, m);
        }
    } else if (m == 1) {
        *b = j + 1;
        name = 1;
    } else {
        name = 0;
    }

    /* stage 2: solve the reduced problem
       recurse if names are not yet unique */
    if (name < m) {
        if (flags & 4) {
            SAIS_MYFREE(C, k, sais_index_type);
        }
        if (flags & 2) {
            SAIS_MYFREE(B, k, sais_index_type);
        }
        newfs = (n + fs) - (m * 2);
        if ((flags & (1 | 4)) == 0) {
            if ((k + name) <= newfs) {
                newfs -= k;
            } else {
                flags |= 8;
            }
        }
        RA = SA + m + newfs;
        if (SA_UINT16_MAX < (name - 1)) { /* char_type = int32_t */
            for (
                i = m + (n >> 1) - 1, j = m - 1; m <= i; --i) {
                if (SA[i] != 0) {
                    assert((char *) (SA + i) <= (char *) (RA + j));
                    RA[j--] = SA[i] - 1;
                }
            }
            if (sais_main_i32(RA, SA, newfs, m, name, 0) != 0) {
                if (flags & 1) {
                    SAIS_MYFREE(C, k, sais_index_type);
                }
                return -2;
            }
        } else if (SA_UINT8_MAX < (name - 1)) { /* char_type = uint16_t */
            sa_uint16_t *RA16;
            newfs =
                (n + fs) -
                (
                    m +
                    (m * sizeof(sa_uint16_t) + sizeof(sa_int32_t) - 1) /
                    sizeof(sa_int32_t));
            if ((flags & (1 | 4)) == 0) {
                if ((k + name) <= newfs) {
                    newfs -= k;
                } else {
                    flags |= 8;
                }
            }
            RA16 = (sa_uint16_t * )(SA + m + newfs);
            for (
                i = m + (n >> 1) - 1, j = m - 1; m <= i; --i) {
                if (SA[i] != 0) {
                    assert((char *) (SA + i) <= (char *) (RA16 + j));
                    RA16[j--] = (sa_uint16_t)(SA[i] - 1);
                }
            }
            if (sais_main_u16(RA16, SA, newfs, m, name, 0) != 0) {
                if (flags & 1) {
                    SAIS_MYFREE(C, k, sais_index_type);
                }
                return -2;
            }
        } else { /* char_type = uint8_t */
            sa_uint8_t *RA8;
            newfs =
                (n + fs) -
                (
                    m +
                    (m * sizeof(sa_uint8_t) + sizeof(sa_int32_t) - 1) /
                    sizeof(sa_int32_t));
            if ((flags & (1 | 4)) == 0) {
                if ((k + name) <= newfs) {
                    newfs -= k;
                } else {
                    flags |= 8;
                }
            }
            RA8 = (sa_uint8_t * )(SA + m + newfs);
            for (
                i = m + (n >> 1) - 1, j = m - 1; m <= i; --i) {
                if (SA[i] != 0) {
                    assert((char *) (SA + i) <= (char *) (RA8 + j));
                    RA8[j--] = (sa_uint8_t)(SA[i] - 1);
                }
            }
            if (sais_main_u8(RA8, SA, newfs, m, name, 0) != 0) {
                if (flags & 1) {
                    SAIS_MYFREE(C, k, sais_index_type);
                }
                return -2;
            }
        }

        i = n - 1;
        j = m - 1;
        c0 = chr(n - 1);
        do {
            c1 = c0;
        } while ((0 <= --i) && ((c0 = chr(i)) >= c1));
        for (
            ; 0 <= i;) {
            do {
                c1 = c0;
            } while ((0 <= --i) && ((c0 = chr(i)) <= c1));
            if (0 <= i) {
                RA[j--] = i + 1;
                do {
                    c1 = c0;
                } while ((0 <= --i) && ((c0 = chr(i)) >= c1));
            }
        }
        for (
            i = 0; i < m; ++i) {
            SA[i] = RA[SA[i]];
        }
        if (flags & 4) {
            if ((C = B = SAIS_MYMALLOC(k, sais_index_type)) == NULL) {
                return -2;
            }
        }
        if (flags & 2) {
            if ((B = SAIS_MYMALLOC(k, sais_index_type)) == NULL) {
                if (flags & 1) {
                    SAIS_MYFREE(C, k, sais_index_type);
                }
                return -2;
            }
        }
    }

    /* stage 3: induce the result for the original problem */
    if (flags & 8) {
        getCounts(T, C, n, k);
    }
    /* put all left-most S characters into their buckets */
    if (1 < m) {
        getBuckets(C, B, k, 1); /* find ends of buckets */
        i = m - 1, j = n, p = SA[m - 1], c1 = chr(p);
        do {
            q = B[c0 = c1];
            while (q < j) {
                SA[--j] = 0;
            }
            do {
                SA[--j] = p;
                if (--i < 0) {
                    break;
                }
                p = SA[i];
            } while ((c1 = chr(p)) == c0);
        } while (0 <= i);
        while (0 < j) {
            SA[--j] = 0;
        }
    }
    if (isbwt == 0) {
        induceSA(T, SA, C, B, n, k);
    } else {
        pidx = computeBWT(T, SA, C, B, n, k);
    }
    if (flags & (1 | 4)) {
        SAIS_MYFREE(C, k, sais_index_type);
    }
    if (flags & 2) {
        SAIS_MYFREE(B, k, sais_index_type);
    }

    return pidx;
}

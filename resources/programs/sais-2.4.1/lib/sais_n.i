/*
 * sais_n.i for sais
 * Copyright (c) 2008-2010 Yuta Mori All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#define CONCATENATE_AGAIN(a, b) a ## b
#define CONCATENATE(a, b) CONCATENATE_AGAIN(a, b)
#define getCounts    CONCATENATE(getCounts_, SAIS_TYPENAME)
#define getBuckets   CONCATENATE(getBuckets_, SAIS_TYPENAME)
#define LMSsort1     CONCATENATE(LMSsort1_, SAIS_TYPENAME)
#define LMSpostproc1 CONCATENATE(LMSpostproc1_, SAIS_TYPENAME)
#define LMSsort2     CONCATENATE(LMSsort2_, SAIS_TYPENAME)
#define LMSpostproc2 CONCATENATE(LMSpostproc2_, SAIS_TYPENAME)
#define induceSA     CONCATENATE(induceSA_, SAIS_TYPENAME)
#define computeBWT   CONCATENATE(computeBWT_, SAIS_TYPENAME)
#define sais_main    CONCATENATE(sais_main_, SAIS_TYPENAME)

/* find the start or end of each bucket */
static void
getCounts(
    const sais_char_type *T, sais_index_type *C, sais_index_type n,
    sais_index_type k) {
    sais_index_type i;
    for (
        i = 0; i < k; ++i) {
        C[i] = 0;
    }
    for (
        i = 0; i < n; ++i) {
        ++C[chr(i)];
    }
}

static void
getBuckets(
    const sais_index_type *C, sais_index_type *B, sais_index_type k,
    sais_bool_type end) {
    sais_index_type i, sum = 0;
    if (end) {
        for (
            i = 0; i < k; ++i) {
            sum += C[i];
            B[i] = sum;
        }
    } else {
        for (
            i = 0; i < k; ++i) {
            sum += C[i];
            B[i] = sum - C[i];
        }
    }
}

/* sort all type LMS suffixes */
static void
LMSsort1(
    const sais_char_type *T, sais_index_type *SA, sais_index_type *C,
    sais_index_type *B, sais_index_type n, sais_index_type k) {
    sais_index_type *b, i, j;
    sais_char_type c0, c1;

    /* compute SAl */
    if (C == B) {
        getCounts(T, C, n, k);
    }
    getBuckets(C, B, k, 0); /* find starts of buckets */
    j = n - 1;
    b = SA + B[c1 = chr(j)];
    --j;
    *b++ = (chr(j) < c1) ? ~j : j;
    for (
        i = 0; i < n; ++i) {
        if (0 < (j = SA[i])) {
            assert(chr(j) >= chr(j + 1));
            if ((c0 = chr(j)) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert(i < (b - SA));
            --j;
            *b++ = (chr(j) < c1) ? ~j : j;
            SA[i] = 0;
        } else if (j < 0) {
            SA[i] = ~j;
        }
    }
    /* compute SAs */
    if (C == B) {
        getCounts(T, C, n, k);
    }
    getBuckets(C, B, k, 1); /* find ends of buckets */
    for (
        i = n - 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            assert(chr(j) <= chr(j + 1));
            if ((c0 = chr(j)) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert((b - SA) <= i);
            --j;
            *--b = (chr(j) > c1) ? ~(j + 1) : j;
            SA[i] = 0;
        }
    }
}

static sais_index_type
LMSpostproc1(
    const sais_char_type *T, sais_index_type *SA, sais_index_type n,
    sais_index_type m) {
    sais_index_type i, j, p, q, plen, qlen, name;
    sais_bool_type diff;
    sais_char_type c0, c1;

    /* compact all the sorted substrings into the first m items of SA
        2*m must be not larger than n (proveable) */
    assert(0 < n);
    for (
        i = 0; (p = SA[i]) < 0; ++i) {
        SA[i] = ~p;
        assert((i + 1) < n);
    }
    if (i < m) {
        for (
            j = i, ++i;; ++i) {
            assert(i < n);
            if ((p = SA[i]) < 0) {
                SA[j++] = ~p;
                SA[i] = 0;
                if (j == m) {
                    break;
                }
            }
        }
    }

    /* store the length of all substrings */
    i = n - 1;
    j = n - 1;
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
            SA[m + ((i + 1) >> 1)] = j - i;
            j = i + 1;
            do {
                c1 = c0;
            } while ((0 <= --i) && ((c0 = chr(i)) >= c1));
        }
    }

    /* find the lexicographic names of all substrings */
    for (
        i = 0, name = 0, q = n, qlen = 0; i < m; ++i) {
        p = SA[i], plen = SA[m + (p >> 1)], diff = 1;
        if ((plen == qlen) && ((q + plen) < n)) {
            for (
                j = 0; (j < plen) && (chr(p + j) == chr(q + j)); ++j) {
            }
            if (j == plen) {
                diff = 0;
            }
        }
        if (diff != 0) {
            ++name, q = p, qlen = plen;
        }
        SA[m + (p >> 1)] = name;
    }

    return name;
}

static void
LMSsort2(
    const sais_char_type *T, sais_index_type *SA, sais_index_type *C,
    sais_index_type *B, sais_index_type *D, sais_index_type n,
    sais_index_type k) {
    sais_index_type *b, i, j, t, d;
    sais_char_type c0, c1;
    assert(C != B);

    /* compute SAl */
    getBuckets(C, B, k, 0); /* find starts of buckets */
    j = n - 1;
    b = SA + B[c1 = chr(j)];
    --j;
    t = (chr(j) < c1);
    j += n;
    *b++ = (t & 1) ? ~j : j;
    for (
        i = 0, d = 0; i < n; ++i) {
        if (0 < (j = SA[i])) {
            if (n <= j) {
                d += 1;
                j -= n;
            }
            assert(chr(j) >= chr(j + 1));
            if ((c0 = chr(j)) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert(i < (b - SA));
            --j;
            t = c0;
            t = (t << 1) | (chr(j) < c1);
            if (D[t] != d) {
                j += n;
                D[t] = d;
            }
            *b++ = (t & 1) ? ~j : j;
            SA[i] = 0;
        } else if (j < 0) {
            SA[i] = ~j;
        }
    }
    for (
        i = n - 1; 0 <= i; --i) {
        if (0 < SA[i]) {
            if (SA[i] < n) {
                SA[i] += n;
                for (
                    j = i - 1; SA[j] < n; --j) {
                }
                SA[j] -= n;
                i = j;
            }
        }
    }

    /* compute SAs */
    getBuckets(C, B, k, 1); /* find ends of buckets */
    for (
        i = n - 1, d += 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            if (n <= j) {
                d += 1;
                j -= n;
            }
            assert(chr(j) <= chr(j + 1));
            if ((c0 = chr(j)) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert((b - SA) <= i);
            --j;
            t = c0;
            t = (t << 1) | (chr(j) > c1);
            if (D[t] != d) {
                j += n;
                D[t] = d;
            }
            *--b = (t & 1) ? ~(j + 1) : j;
            SA[i] = 0;
        }
    }
}

static sais_index_type
LMSpostproc2(sais_index_type *SA, sais_index_type n, sais_index_type m) {
    sais_index_type i, j, d, name;

    /* compact all the sorted LMS substrings into the first m items of SA */
    assert(0 < n);
    for (
        i = 0, name = 0; (j = SA[i]) < 0; ++i) {
        j = ~j;
        if (n <= j) {
            name += 1;
        }
        SA[i] = j;
        assert((i + 1) < n);
    }
    if (i < m) {
        for (
            d = i, ++i;; ++i) {
            assert(i < n);
            if ((j = SA[i]) < 0) {
                j = ~j;
                if (n <= j) {
                    name += 1;
                }
                SA[d++] = j;
                SA[i] = 0;
                if (d == m) {
                    break;
                }
            }
        }
    }
    if (name < m) {
        /* store the lexicographic names */
        for (
            i = m - 1, d = name + 1; 0 <= i; --i) {
            if (n <= (j = SA[i])) {
                j -= n;
                --d;
            }
            SA[m + (j >> 1)] = d;
        }
    } else {
        /* unset flags */
        for (
            i = 0; i < m; ++i) {
            if (n <= (j = SA[i])) {
                j -= n;
                SA[i] = j;
            }
        }
    }

    return name;
}

/* compute SA and BWT */
static void
induceSA(
    const sais_char_type *T, sais_index_type *SA, sais_index_type *C,
    sais_index_type *B, sais_index_type n, sais_index_type k) {
    sais_index_type *b, i, j;
    sais_char_type c0, c1;
    /* compute SAl */
    if (C == B) {
        getCounts(T, C, n, k);
    }
    getBuckets(C, B, k, 0); /* find starts of buckets */
    j = n - 1;
    b = SA + B[c1 = chr(j)];
    *b++ = ((0 < j) && (chr(j - 1) < c1)) ? ~j : j;
    for (
        i = 0; i < n; ++i) {
        j = SA[i], SA[i] = ~j;
        if (0 < j) {
            --j;
            assert(chr(j) >= chr(j + 1));
            if ((c0 = chr(j)) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert(i < (b - SA));
            *b++ = ((0 < j) && (chr(j - 1) < c1)) ? ~j : j;
        }
    }
    /* compute SAs */
    if (C == B) {
        getCounts(T, C, n, k);
    }
    getBuckets(C, B, k, 1); /* find ends of buckets */
    for (
        i = n - 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            --j;
            assert(chr(j) <= chr(j + 1));
            if ((c0 = chr(j)) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert((b - SA) <= i);
            *--b = ((j == 0) || (chr(j - 1) > c1)) ? ~j : j;
        } else {
            SA[i] = ~j;
        }
    }
}

static sais_index_type
computeBWT(
    const sais_char_type *T, sais_index_type *SA, sais_index_type *C,
    sais_index_type *B, sais_index_type n, sais_index_type k) {
    sais_index_type *b, i, j, pidx = -1;
    sais_char_type c0, c1;
    /* compute SAl */
    if (C == B) {
        getCounts(T, C, n, k);
    }
    getBuckets(C, B, k, 0); /* find starts of buckets */
    j = n - 1;
    b = SA + B[c1 = chr(j)];
    *b++ = ((0 < j) && (chr(j - 1) < c1)) ? ~j : j;
    for (
        i = 0; i < n; ++i) {
        if (0 < (j = SA[i])) {
            --j;
            assert(chr(j) >= chr(j + 1));
            SA[i] = ~((sais_index_type)(c0 = chr(j)));
            if (c0 != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert(i < (b - SA));
            *b++ = ((0 < j) && (chr(j - 1) < c1)) ? ~j : j;
        } else if (j != 0) {
            SA[i] = ~j;
        }
    }
    /* compute SAs */
    if (C == B) {
        getCounts(T, C, n, k);
    }
    getBuckets(C, B, k, 1); /* find ends of buckets */
    for (
        i = n - 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            --j;
            assert(chr(j) <= chr(j + 1));
            SA[i] = (c0 = chr(j));
            if (c0 != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            assert((b - SA) <= i);
            *--b =
                ((0 < j) && (chr(j - 1) > c1)) ? ~((sais_index_type) chr(
                    j - 1)) : j;
        } else if (j != 0) {
            SA[i] = ~j;
        } else {
            pidx = i;
        }
    }
    return pidx;
}



#undef CONCATENATE_AGAIN
#undef CONCATENATE
#undef getCounts
#undef getBuckets
#undef stage1slowsort
#undef stage1fastsort
#undef induceSA
#undef computeBWT
#undef sais_main

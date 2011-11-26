#include <stdint.h>
#include <stdbool.h>
#include <Python.h>

static uint64_t
powmod(uint64_t a, uint64_t d, uint64_t n)
{
    if (d == 0) return 1;
    if (d == 1) return a;
    if (d % 2 == 0)
        return powmod(a * a % n, d / 2, n) % n;
    else
        return powmod(a * a % n, d / 2, n) * a % n;
}

static bool
millerRabin(uint64_t n, uint64_t a)
{
    if (n % 2 == 0)
        return false;
    uint64_t d = n - 1;
    while (d % 2 == 0)
        d /= 2;
    uint64_t t = powmod(a, d, n);
    while (d != n - 1 && t != 1 && t != n - 1)
    {
        t = t * t % n;
        d *= 2;
    }
    return t == n - 1 || d % 2 == 1;
}

static bool
isPrime(uint64_t n)
{
    if (n == 0 || n == 1 || n == 4)
        return false;
    if (n <= 5) return true;
    if (n < 1373653) {
        return millerRabin(n, 2) &&
               millerRabin(n, 3);
    }
    if (n < 9080191) {
        return millerRabin(n, 31) &&
               millerRabin(n, 73);
    }
    if (n < 4759123141ll) {
        return millerRabin(n, 2) &&
               millerRabin(n, 7) &&
               millerRabin(n, 61);
    }
    // FIXME current, it cannot check a number
    //       over 2^32-1 since it might overflow
    return false;
}

static PyObject *
Prime_IsPrime(PyObject *self, PyObject *args)
{
    uint64_t n;
    if (! PyArg_ParseTuple(args, "K", &n))
        return NULL;

    if (n >= (1ll << 32)) {
        PyErr_SetString(PyExc_OverflowError,
                "Cannot check a number over 2 ** 32 - 1");
        return NULL;
    }

    if (isPrime(n))
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyMethodDef PrimeMethods[] = {
    {"is_prime", Prime_IsPrime, METH_VARARGS,
        "Check if a number a prime."},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initprime(void)
{
    (void) Py_InitModule("prime", PrimeMethods);
}

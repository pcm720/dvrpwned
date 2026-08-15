#!/usr/bin/env bash
set -euo pipefail

# Build a fr30-elf cross toolchain (binutils + GCC) from upstream sources.
# Install prefix: tools/gcc-fr30-elf/

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
PREFIX="$ROOT/tools/gcc-fr30-elf"
SRC_DIR="$ROOT/build/toolchain-src"
BUILD_DIR="$ROOT/build/toolchain-build"

BINUTILS_VERSION="2.47"
GCC_VERSION="16.2.0"

BINUTILS_TAR="binutils-${BINUTILS_VERSION}.tar.xz"
GCC_TAR="gcc-${GCC_VERSION}.tar.xz"

MIRROR="https://mirrors.kernel.org/gnu"
BINUTILS_URL="${MIRROR}/binutils/${BINUTILS_TAR}"
GCC_URL="${MIRROR}/gcc/gcc-${GCC_VERSION}/${GCC_TAR}"

JOBS="$(nproc 2>/dev/null || echo 4)"

mkdir -p "$SRC_DIR" "$BUILD_DIR" "$PREFIX"

cd "$SRC_DIR"

if [[ ! -f "$BINUTILS_TAR" ]]; then
    echo "[+] Downloading binutils ${BINUTILS_VERSION}"
    curl -L -o "$BINUTILS_TAR" "$BINUTILS_URL"
fi
if [[ ! -d "binutils-${BINUTILS_VERSION}" ]]; then
    echo "[+] Extracting binutils"
    tar -xf "$BINUTILS_TAR"
fi

if [[ ! -f "$GCC_TAR" ]]; then
    echo "[+] Downloading GCC ${GCC_VERSION}"
    curl -L -o "$GCC_TAR" "$GCC_URL"
fi
if [[ ! -d "gcc-${GCC_VERSION}" ]]; then
    echo "[+] Extracting GCC"
    tar -xf "$GCC_TAR"
fi

echo "[+] Building binutils"
rm -rf "$BUILD_DIR/binutils"
mkdir -p "$BUILD_DIR/binutils"
cd "$BUILD_DIR/binutils"
"$SRC_DIR/binutils-${BINUTILS_VERSION}/configure" \
    --target=fr30-elf \
    --prefix="$PREFIX" \
    --disable-nls \
    --disable-werror \
    --with-sysroot
make -j"$JOBS"
make install

echo "[+] Building GCC (stage 1, no libc)"
export PATH="$PREFIX/bin:$PATH"
rm -rf "$BUILD_DIR/gcc"
mkdir -p "$BUILD_DIR/gcc"
cd "$BUILD_DIR/gcc"
export CFLAGS="-O2 -g"
export CXXFLAGS="-O2 -g -std=gnu++17"
"$SRC_DIR/gcc-${GCC_VERSION}/configure" \
    --target=fr30-elf \
    --prefix="$PREFIX" \
    --enable-languages=c \
    --without-headers \
    --disable-nls \
    --disable-libssp \
    --disable-threads \
    --disable-libatomic \
    --disable-libitm \
    --disable-libstdcxx \
    --disable-shared \
    --disable-multilib \
    --disable-libcody
make -j"$JOBS" all-gcc
make install-gcc
make -j"$JOBS" all-target-libgcc
make install-target-libgcc

echo "[+] Done. Toolchain installed to $PREFIX"
echo "[+] Add to PATH: export PATH=\"$PREFIX/bin:\$PATH\""

"$PREFIX/bin/fr30-elf-gcc" --version
"$PREFIX/bin/fr30-elf-as" --version
"$PREFIX/bin/fr30-elf-ld" --version

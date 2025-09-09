FROM debian:stable-slim

RUN apt-get update && apt-get install -y --no-install-recommends     build-essential cmake ninja-build python3 && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

# Build and run tests in Release by default
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DQUASAR_ENABLE_TESTS=ON &&     cmake --build build -j &&     ctest --test-dir build --output-on-failure

CMD ["bash"]
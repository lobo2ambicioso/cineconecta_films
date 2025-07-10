FROM ubuntu:22.04

# 1. Instalar dependencias necesarias del sistema
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git \
  curl \
  ninja-build \
  python3 \
  python3-pip \
  libssl-dev \
  libcurl4-openssl-dev \
  libpq-dev \
  libpqxx-dev \
  libmysqlcppconn-dev \
  pkg-config

# 2. Instalar Meson (sistema de build para Pistache)
RUN pip3 install meson

# 3. Clonar y construir nlohmann_json correctamente con CMake
WORKDIR /tmp
RUN git clone https://github.com/nlohmann/json.git && \
  cd json && \
  mkdir build && cd build && \
  cmake .. && \
  make && \
  make install

# 4. Clonar e instalar Pistache desde fuente con Meson
RUN git clone https://github.com/pistacheio/pistache.git && \
  cd pistache && \
  meson setup build && \
  meson compile -C build && \
  meson install -C build && \
  ldconfig

# 5. Copiar código de la app
WORKDIR /app
COPY src/ ./src
COPY CMakeLists.txt ./CMakeLists.txt

# 6. Compilar la app con CMake
RUN mkdir build && cd build && \
  cmake .. && \
  make && \
  ls -la

# 7. Exponer puerto 5000
EXPOSE 5000

# 8. Comando para ejecutar tu app correctamente
CMD ["./build/cineconecta"]

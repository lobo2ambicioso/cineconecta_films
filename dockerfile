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

# 3. Descargar json.hpp (nlohmann)
WORKDIR /tmp
RUN curl -L -o json.hpp https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp && \
  mkdir -p /usr/local/include/nlohmann && \
  mv json.hpp /usr/local/include/nlohmann/json.hpp

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
  make

# 7. Exponer puerto 5555
EXPOSE 5555

# 8. Comando para ejecutar tu app correctamente
CMD ["./build/ServiceCPP_Executable"]

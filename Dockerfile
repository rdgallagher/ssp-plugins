FROM dockcross/linux-armv7a

ENV DEFAULT_DOCKCROSS_IMAGE rdgallagher/ssp-vst
RUN apt-get install && apt-get install -y \
  clang \
  git \
  ladspa-sdk \
  freeglut3-dev \
  g++ \
  libasound2-dev \
  libcurl4-openssl-dev \
  libfreetype6-dev \
  libjack-jackd2-dev \
  libx11-dev \
  libxcomposite-dev \
  libxcursor-dev \
  libxinerama-dev \
  libxrandr-dev \
  mesa-common-dev \
  webkit2gtk-4.0 \
  juce-tools

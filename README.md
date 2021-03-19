# SSP Plugins
Plugins for the Percussa SSP

Visit http://www.percussa.com/ for more info about the Percussa SSP Eurorack module. For any questions visit the forum at http://forum.percussa.com/ 

### Build and install plugins

Start in this repo...
- `cd ssp-plugins`

#### Download dependencies (one time setup)
- `git submodule init`
- `git submodule update`

#### Download Docker image for cross-compilation (one time setup)
- `docker run --rm rdgallagher/ssp-vst > dockcross`
- `chmod +x dockcross`
- `mv dockcross ~/bin/` or elsewhere on your PATH

#### Build plugin for SSP
- `dockcross bash -c 'cd plugins/scp/Builds/Linux && make CONFIG=Release'`

#### SCP uhh... SCP plugin to SSP
- `scp plugins/scp/Builds/Linux/build/Debug/scp.so linaro@192.168.0.6:/media/linaro/SYNTHOR/plugins`

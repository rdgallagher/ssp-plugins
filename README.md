# SSP Plugins
Plugins for the Percussa SSP

Visit http://www.percussa.com/ for more info about the Percussa SSP Eurorack module. For any questions visit the forum at http://forum.percussa.com/ 

### Development

#### Get Dependencies
- `git submodule init`
- `git submodule update`

#### Build Docker image for cross-compilation
- `docker build ssp-vst .`
- `docker run --rm ssp-vst > dockcross`
- `chmod +x dockcross`
- `mv dockcross ~/bin/` or elsewhere on your PATH

#### Build plugin for SSP
- `dockcross bash -c 'cd plugins/scp/Builds/Linux && make CONFIG=Release'`

#### SCP uhh... SCP plugin to SSP
- ` scp plugins/scp/Builds/Linux/build/Debug/scp.so linaro@192.168.0.6:/media/linaro/SYNTHOR/plugins`

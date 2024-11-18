# openCL

# Primeiros comandos foram retirados deste Getting start:
https://github.com/KhronosGroup/OpenCL-Guide/blob/main/chapters/getting_started_linux.md


Feito isso foi necesssario seguir os outros passos:
# Instalado ROCm


Comandos:
wget -qO - http://repo.radeon.com/rocm/rocm.gpg.key | sudo apt-key add -
echo 'deb [arch=amd64] http://repo.radeon.com/rocm/apt/debian/ ubuntu main' | sudo tee /etc/apt/sources.list.d/rocm.list



sudo apt update
sudo apt install rocm-opencl rocm-opencl-dev -y

#Instalar o mesa-open-cl, visto que a minha placa de video e antiga

sudo apt install -f --fix-missing rocm-dkms
sudo apt install mesa-opencl-icd

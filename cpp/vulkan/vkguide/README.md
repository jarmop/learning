Install Vulkan SDK:
```sh
mkdir -p ~/development/lib/vulkansdk
wget https://sdk.lunarg.com/sdk/download/1.4.341.1/linux/vulkansdk-linux-x86_64-1.4.341.1.tar.xz
tar -xf vulkansdk-linux-x86_64-1.4.341.1.tar.xz -C ~/development/lib/vulkansdk
cd ~/development/lib/vulkansdk
ln -s 1.4.341.1 default
```

Add Vulkan SDK setup path to ".zshrc":
```sh
source ~/development/lib/vulkansdk/default/setup-env.sh
```

Run:
```sh
./run.sh
```
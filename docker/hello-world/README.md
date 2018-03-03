#Hello World app with docker

https://docs.docker.com/get-started/part2/

Build and run
```
docker build -t friendlyhello .
docker run [-d] -p 4000:80 friendlyhello
```
http://localhost:4000/

Shutdown
```
docker ps
docker container stop <CONTAINER ID>
```

Tag and push to repository
```
docker tag friendlyhello jarmop/get-started:part2
docker image ls
docker push jarmop/get-started:part2
docker pull jarmop/get-started
```

Run from repository (pulls if necessary)
```
docker run -p 4000:80 jarmop/get-started:part2
```

## Used commands and some related ones
```
docker build -t friendlyhello .  # Create image using this directory's Dockerfile
docker run -p 4000:80 friendlyhello  # Run "friendlyname" mapping port 4000 to 80
docker run -d -p 4000:80 friendlyhello         # Same thing, but in detached mode
docker container ls                                # List all running containers
docker container ls -a             # List all containers, even those not running
docker container stop <hash>           # Gracefully stop the specified container
docker container kill <hash>         # Force shutdown of the specified container
docker container rm <hash>        # Remove specified container from this machine
docker container rm $(docker container ls -a -q)         # Remove all containers
docker image ls -a                             # List all images on this machine
docker image rm <image id>            # Remove specified image from this machine
docker image rm $(docker image ls -a -q)   # Remove all images from this machine
docker login             # Log in this CLI session using your Docker credentials
docker tag <image> username/repository:tag  # Tag <image> for upload to registry
docker push username/repository:tag            # Upload tagged image to registry
docker run username/repository:tag                   # Run image from a registry
```
FROM node:20

# Set the working directory to /app
# WORKDIR /app

# Copy the current directory contents into the container at /app
# ADD . /app

# This doesn't really do anything, it's just a documentation about
# what ports are intended to be exposed with the "run" command
EXPOSE 3000

COPY . .

CMD ["node", "test.js"]
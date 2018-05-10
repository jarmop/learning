// This is for running loaders manually

const fs = require("fs");
const path = require("path");
const { runLoaders } = require("loader-runner");

runLoaders(
    {
      resource: "./asset.txt",
      loaders: [path.resolve(__dirname, "./custom-loader")],
      readResource: fs.readFile.bind(fs),
    },
    (err, result) => (err ? console.error(err) : console.log(result))
);
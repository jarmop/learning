const HtmlWebpackPlugin = require('html-webpack-plugin');
// var path = require("path");

module.exports = {
  mode: 'production',
  entry: './app.js',
  output: {
    filename: 'bundle.js'
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: 'index.html',
    }),
  ],
  devServer: {
    // contentBase: path.resolve(__dirname, 'dist'),
    // Display only errors to reduce the amount of output.
    //stats: "errors-only",
    // host: process.env.HOST, // Defaults to `localhost`
    // port: process.env.PORT, // Defaults to 8080
    open: true, // Open the page in browser
  },
  module: {
    rules: [
      {
        test: /\.scss$/,
        // include,
        // exclude,
        use: [
          'style-loader',
          'css-loader',
          'sass-loader',
        ],
      },
    ],
  },
};
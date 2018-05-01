const HtmlWebpackPlugin = require("html-webpack-plugin");

module.exports = {
  devtool: false, // Disable source maps to make output files easier to explore
  plugins: [
    new HtmlWebpackPlugin({
      template: 'index.html',
    }),
  ],
};
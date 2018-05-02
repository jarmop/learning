const HtmlWebpackPlugin = require('html-webpack-plugin');
const merge = require("webpack-merge");

const parts = require("./webpack.parts");

const commonConfig = merge([
  {
    devtool: false,
    plugins: [
      new HtmlWebpackPlugin({
        title: "Webpack demo",
      }),
    ],
  },
]);

const productionConfig = merge([
  parts.extractCSS({
    use: 'css-loader',
  })
]);

const developmentConfig = merge([
  parts.devServer({
    // Customize host/port here if needed
    host: process.env.HOST,
    port: process.env.PORT,
  }),
  parts.loadCSS(),
]);

module.exports = mode => {
  if (mode === "production") {
    return merge(commonConfig, productionConfig, {mode});
  }

  return merge(commonConfig, developmentConfig, {mode});
};
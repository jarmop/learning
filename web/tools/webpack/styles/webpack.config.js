const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
  mode: 'none',
  devtool: false, // disable source maps
  entry: './src/foo.js',
  plugins: [
    new HtmlWebpackPlugin({
      template: 'src/index.html',
    }),
  ],
  devServer: {
    open: true,
  },
  module: {
    rules: [
      {
        test: /\.scss$/,
        use: [
          'style-loader',
          'css-loader',
          'sass-loader',
        ],
      },
    ],
  },
};
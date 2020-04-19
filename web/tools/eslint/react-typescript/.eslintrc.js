module.exports = {
  env: {
    browser: true,
    es6: true,
  },
  extends: [
    'plugin:react/recommended',
    'airbnb',
  ],
  globals: {
    Atomics: 'readonly',
    SharedArrayBuffer: 'readonly',
  },
  parser: '@typescript-eslint/parser',
  parserOptions: {
    ecmaFeatures: {
      jsx: true,
    },
    ecmaVersion: 2018,
    sourceType: 'module',
  },
  plugins: [
    'react',
    '@typescript-eslint',
  ],
  rules: {
    'react/jsx-filename-extension': [
      2,
      {
        extensions: ['.tsx']
      }
    ],
    // prohibit extensions in imports (2 = triggers error)
    'import/extensions': [
      2,
      'never',
    ],
    // prohibit semicolons
    'semi': [
      2,
      'never'
    ],
    // allow multiple jsx expressions per line
    'react/jsx-one-expression-per-line': 0,
  },
  settings: {
    react: {
      version: 'detect',
    }
  }
};

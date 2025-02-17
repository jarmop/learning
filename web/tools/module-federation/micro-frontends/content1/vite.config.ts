import { federation } from '@module-federation/vite'
import react from '@vitejs/plugin-react'
import { writeFileSync } from 'fs'
import { defineConfig, loadEnv } from 'vite'
import { dependencies } from './package.json'

export default defineConfig(({ mode }) => {
  const selfEnv = loadEnv(mode, process.cwd())
  return {
    server: {
      fs: {
        allow: ['.'],
      },
    },
    // preview: {
    //   allowedHosts: true,
    //   cors: 'http://www.jarmo.com',
    // },
    build: {
      target: 'chrome89',
    },
    plugins: [
      {
        name: 'generate-environment',
        options: function () {
          console.info('selfEnv', selfEnv)
          writeFileSync('./src/environment.ts', `export default ${JSON.stringify(selfEnv, null, 2)};`)
        },
      },
      federation({
        filename: 'remoteEntry.js',
        name: 'content1',
        exposes: {
          './remote-app': './src/App.tsx',
        },
        remotes: {},
        shared: {
          react: {
            requiredVersion: dependencies.react,
            singleton: true,
          },
        },
      }),
      react(),
    ],
  }
})

import { nxE2EPreset } from '@nx/cypress/plugins/cypress-preset'

import { defineConfig } from 'cypress'

export default defineConfig({
  e2e: {
    ...nxE2EPreset(__filename, {
      cypressDir: 'src',
      webServerCommands: { default: 'nx run first-app:serve', production: 'nx run first-app:preview' },
      ciWebServerCommand: 'nx run first-app:serve-static',
    }),
    baseUrl: 'http://localhost:4200',
  },
})

import { lazy, Suspense } from 'react'

const Remote = lazy(
  // @ts-expect-error module-federation stuff
  async () => import('content1/remote-app')
)

const App = () => {
  return (
    <>
      <p>Loading remote counter under this</p>
      <Suspense fallback="loading...">
        <Remote />
      </Suspense>
    </>
  )
}

export default App

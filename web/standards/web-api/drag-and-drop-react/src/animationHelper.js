export const animation = () => {
  let element = document.getElementById('element')

  let frameCount = 0
  const animate = (maxFrames) => {
    element.style.height = element.clientHeight + 1 + 'px'
    frameCount++
    console.log(frameCount)

    if (frameCount < maxFrames) {
      console.log('requestAnimationFrame')
      window.requestAnimationFrame(() => animate(maxFrames))
    }
  }

  animate(100)
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms))
}

export async function demo() {
  setTimeout(() => {
    console.log('after one second')
  }, 1000)
  console.log('Taking a break...')
  await sleep(2000)
  console.log('Two second later')
}

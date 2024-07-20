import { WebGLRenderer, PerspectiveCamera, Scene, Color, TextureLoader, RepeatWrapping, NearestFilter, SRGBColorSpace, PlaneGeometry, MeshPhongMaterial, DoubleSide, Mesh, BoxGeometry, SphereGeometry, AmbientLight } from 'three'
import { OrbitControls } from 'three/addons/controls/OrbitControls.js'

export function main() {
  const canvas = document.querySelector<HTMLCanvasElement>('#c')
  if (!canvas) return
  const renderer = new WebGLRenderer({ antialias: true, canvas })

  const fov = 45
  const aspect = 2 // the canvas default
  const near = 0.1
  const far = 100
  const camera = new PerspectiveCamera(fov, aspect, near, far)
  camera.position.set(0, 10, 20)

  const controls = new OrbitControls(camera, canvas)
  controls.target.set(0, 5, 0)
  controls.update()

  const scene = new Scene()
  scene.background = new Color('black')

  {
    const planeSize = 40

    const loader = new TextureLoader()
    const texture = loader.load('https://threejs.org/manual/examples/resources/images/checker.png')
    texture.wrapS = RepeatWrapping
    texture.wrapT = RepeatWrapping
    texture.magFilter = NearestFilter
    texture.colorSpace = SRGBColorSpace
    const repeats = planeSize / 2
    texture.repeat.set(repeats, repeats)

    const planeGeo = new PlaneGeometry(planeSize, planeSize)
    const planeMat = new MeshPhongMaterial({
      map: texture,
      side: DoubleSide,
    })
    const mesh = new Mesh(planeGeo, planeMat)
    mesh.rotation.x = Math.PI * -0.5
    scene.add(mesh)
  }

  {
    const cubeSize = 4
    const cubeGeo = new BoxGeometry(cubeSize, cubeSize, cubeSize)
    const cubeMat = new MeshPhongMaterial({ color: '#8AC' })
    const mesh = new Mesh(cubeGeo, cubeMat)
    mesh.position.set(cubeSize + 1, cubeSize / 2, 0)
    scene.add(mesh)
  }

  {
    const sphereRadius = 3
    const sphereWidthDivisions = 32
    const sphereHeightDivisions = 16
    const sphereGeo = new SphereGeometry(sphereRadius, sphereWidthDivisions, sphereHeightDivisions)
    const sphereMat = new MeshPhongMaterial({ color: '#CA8' })
    const mesh = new Mesh(sphereGeo, sphereMat)
    mesh.position.set(-sphereRadius - 1, sphereRadius + 2, 0)
    scene.add(mesh)
  }

  // class ColorGUIHelper {
  //   constructor(object, prop) {
  //     this.object = object
  //     this.prop = prop
  //   }
  //   get value() {
  //     return `#${this.object[this.prop].getHexString()}`
  //   }
  //   set value(hexString) {
  //     this.object[this.prop].set(hexString)
  //   }
  // }

  {
    const color = 0xffffff
    const intensity = 1
    const light = new AmbientLight(color, intensity)
    scene.add(light)

    // const gui = new GUI()
    // gui.addColor(new ColorGUIHelper(light, 'color'), 'value').name('color')
    // gui.add(light, 'intensity', 0, 5, 0.01)
  }

  function resizeRendererToDisplaySize(renderer: WebGLRenderer) {
    const canvas = renderer.domElement
    const width = canvas.clientWidth
    const height = canvas.clientHeight
    const needResize = canvas.width !== width || canvas.height !== height
    if (needResize) {
      renderer.setSize(width, height, false)
    }

    return needResize
  }

  function render() {
    if (resizeRendererToDisplaySize(renderer)) {
      const canvas = renderer.domElement
      camera.aspect = canvas.clientWidth / canvas.clientHeight
      camera.updateProjectionMatrix()
    }

    renderer.render(scene, camera)

    requestAnimationFrame(render)
  }

  requestAnimationFrame(render)
}

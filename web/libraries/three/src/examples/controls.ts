import {
  WebGLRenderer,
  PerspectiveCamera,
  Scene,
  Color,
  MeshPhongMaterial,
  Mesh,
  BoxGeometry,
  AmbientLight,
  Camera,
  DoubleSide,
  NearestFilter,
  PlaneGeometry,
  RepeatWrapping,
  SRGBColorSpace,
  TextureLoader,
} from 'three'
import { OrbitControls } from 'three/addons/controls/OrbitControls.js'

const aspectRatio = 16 / 9
const width = 1600
const height = width / aspectRatio

function createRenderer() {
  const renderer = new WebGLRenderer({ antialias: true })
  renderer.setSize(width, height)
  document.body.appendChild(renderer.domElement)

  return renderer
}

function createCamera() {
  const fieldOfView = 75
  const near = 0.1
  const far = 100
  const camera = new PerspectiveCamera(fieldOfView, aspectRatio, near, far)

  camera.position.set(0, 10, 20)

  return camera
}

function animateControls(renderer: WebGLRenderer, camera: PerspectiveCamera, scene: Scene) {
  function render() {
    renderer.render(scene, camera)

    requestAnimationFrame(render)
  }

  requestAnimationFrame(render)
}

function addControls(camera: Camera, canvas: HTMLCanvasElement) {
  const controls = new OrbitControls(camera, canvas)
  controls.target.set(0, 5, 0)
  controls.update()
}

function createPlane() {
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
  return mesh
}

export function main() {
  const renderer = createRenderer()
  const camera = createCamera()

  addControls(camera, renderer.domElement)

  const scene = new Scene()
  scene.background = new Color('black')

  const plane = createPlane()
  scene.add(plane)

  const cubeSize = 4
  const cubeGeo = new BoxGeometry(cubeSize, cubeSize, cubeSize)
  const cubeMat = new MeshPhongMaterial({ color: '#8AC' })
  const mesh = new Mesh(cubeGeo, cubeMat)
  mesh.position.set(cubeSize + 1, cubeSize / 2, 0)
  scene.add(mesh)

  const color = new Color(0xffffff)
  const intensity = 1
  const light = new AmbientLight(color, intensity)
  scene.add(light)

  animateControls(renderer, camera, scene)
}

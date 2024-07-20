import {
  WebGLRenderer,
  PerspectiveCamera,
  Scene,
  DirectionalLight,
  BoxGeometry,
  Mesh,
  MeshPhongMaterial,
  Color,
  ColorRepresentation,
  Camera,
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

  camera.position.z = 15

  return camera
}

function createScene() {
  const scene = new Scene()
  scene.background = new Color(255, 255, 255)
  const color = 0xffffff
  const intensity = 3
  const light = new DirectionalLight(color, intensity)
  light.position.set(-1, 2, 4)
  scene.add(light)

  return scene
}

function createCube(geometry: BoxGeometry, color: ColorRepresentation, x: number) {
  const material = new MeshPhongMaterial({ color })
  const cube = new Mesh(geometry, material)
  cube.position.x = x

  return cube
}

function createCubes() {
  const geometry = new BoxGeometry(1, 1, 1)

  const cubes = [
    createCube(geometry, 0x44aa88, 0),
    createCube(geometry, 0x8844aa, -2),
    createCube(geometry, 0xaa8844, 2),
  ]

  return cubes
}

function animate(renderer: WebGLRenderer, camera: Camera, scene: Scene, cubes: Mesh[]) {
  function render(time: number) {
    time /= 1000 // convert time to seconds

    cubes.forEach((cube, i) => {
      const speed = 1 + i * 0.1
      const rotation = time * speed
      cube.rotation.x = rotation
      cube.rotation.y = rotation
    })

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

export function main() {
  const renderer = createRenderer()
  const camera = createCamera()
  addControls(camera, renderer.domElement)
  const scene = createScene()
  const cubes = createCubes()

  cubes.forEach((cube) => scene.add(cube))

  // renderer.render(scene, camera)

  animate(renderer, camera, scene, cubes)
  // animateControls(renderer, camera, scene)
}

import {
  WebGLRenderer,
  PerspectiveCamera,
  Scene,
  DirectionalLight,
  Mesh,
  Color,
  PlaneGeometry,
  MeshBasicMaterial,
} from 'three'
import './style.css'

const aspectRatio = 1
const width = 800
const height = width / aspectRatio

function createRenderer() {
  const renderer = new WebGLRenderer({ antialias: true })
  renderer.setSize(width, height)
  document.body.appendChild(renderer.domElement)

  return renderer
}

function createCamera(aspectRatio: number) {
  const fieldOfView = 150
  const near = 0.1
  const far = 100
  const camera = new PerspectiveCamera(fieldOfView, aspectRatio, near, far)

  camera.position.z = 30

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

const bayGeometry = new PlaneGeometry(100, 200)
const notchGeometry = new PlaneGeometry(2, 4)

function createBay(x: number, y: number) {
  const material = new MeshBasicMaterial({ color: 0x44aa88 })
  const plane = new Mesh(bayGeometry, material)
  plane.position.x = x
  plane.position.y = y

  return plane
}

function createNotch(x: number, y: number) {
  const material = new MeshBasicMaterial({ color: 0x000000 })
  const plane = new Mesh(notchGeometry, material)
  plane.position.x = x
  plane.position.y = y

  return plane
}

function createMeshes() {
  const bay = createBay(0, 0)
  const notches = [createNotch(-49, 4), createNotch(0, 0), createNotch(-4, 4), createNotch(-4, 4)]
  notches.forEach(notch => bay.add(notch))
  const meshes = [bay]

  return meshes
}

export function main() {
  const renderer = createRenderer()
  const camera = createCamera(aspectRatio)
  const scene = createScene()
  const meshes = createMeshes()

  meshes.forEach((mesh) => scene.add(mesh))

  renderer.render(scene, camera)
}

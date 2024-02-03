export function replaceAtIndex<T>(arr: T[], index: number, value: T) {
  return arr.map((oldValue, oldIndex) => (oldIndex === index ? value : oldValue))
}
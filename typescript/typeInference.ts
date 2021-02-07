const returni = () => {
  if (true) {
    return 1
  }
  return 'gre'
}

export default () => {
  const value = returni()
  // let value = returni() // let becomes string | number whereas const type is the exact values
}

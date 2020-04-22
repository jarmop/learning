import * as React from 'react'
import SpecialComponent from './SpecialComponent/SpecialComponent'

export interface HelloProps {
  compiler: string;
  framework: string;
}

export const Hello = ({
  compiler,
  framework,
}: HelloProps) => (
  <h1>
    Hello from {compiler} and {framework}!
    <SpecialComponent/>
  </h1>
)

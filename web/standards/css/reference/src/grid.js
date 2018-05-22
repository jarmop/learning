import React, { Component } from 'react';

class Grid extends Component {
  render() {
    let {gridStyle, cellStyle} = this.props.styles;

    let cells = [];
    for (let i = 0; i < 9; i++) {
      cells.push(
          <div key={i} className="cell" style={cellStyle}>{i}</div>
      );
    }

    return (
        <div className="grid" style={gridStyle}>
          {cells}
        </div>
    );
  }
}

export default Grid;
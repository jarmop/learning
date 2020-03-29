import React, { useState } from 'react';

 /**
  * If index is used as a key, component won't get
  * rerendered when list order is changed.
  */ 

const initialData = [
    {
        id: 1,
        value: 1,
        name: 'initial1'
    },
    {
        id: 2,
        value: 2,
        name: 'initial2',
    },
];
  
const sortByValue = (a, b) => a.value - b.value;

function ListKey() {
    const [data, setData] = useState(initialData)

    const updateData = (updatedDatum, updatedIndex) => {
      const datumAdded = data.map((datum, index) => 
        index === updatedIndex ? updatedDatum : datum
      )
      const sortedData = datumAdded.sort(sortByValue)
  
      setData(sortedData)
    }

    return (
        <div>
            <h1>React list key</h1>
            {data.map((datum, index) => 
                <Datum
                    // key={index}
                    key={datum.id}
                    datum={datum} 
                    onSave={datum => updateData(datum, index)}
                />
        
            )}
        </div>
    );
}

function Datum({datum, onSave}) {
    const save = value => {
        onSave({...datum, value,});
    }
    return (
        <div>
            <input 
                type="number" 
                defaultValue={datum.value}
                onChange={event => save(parseInt(event.target.value))}  
            />
            {' - '}
            {datum.name}
        </div>
    )
}

export default ListKey;
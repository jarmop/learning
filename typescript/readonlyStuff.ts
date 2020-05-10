function readonlyTuple() {
    const foo: readonly [string, number] = ['foo', 5];
    // foo[0] = 'bar'; // error
}

function readonlyArray() {
    const rarr: ReadonlyArray<number> = [1, 2, 3];
    // rarr[1] = 5; // error

    const rarr2: readonly number[] = [10, 20, 30];
    // rarr2[2] = 7; // error
}

function readonlyProperty() {
    interface Point {
        readonly x: number;
        readonly y: number;
    }
    let p1: Point = { x: 10, y: 20 };
    // p1.x = 5; // error

    class Octopus {
        readonly name: string;
        readonly numberOfLegs: number = 8;
        constructor(theName: string) {
            this.name = theName;
        }
    }

    const dad = new Octopus("Man with the 8 strong legs");
    // dad.name = "Man with the 3-piece suit"; // error
}

function readonlyUtilityType() {
    /** 
    * Readonly<T> only prevents assigning new values to properties. 
    * So to make an object type deep readonly you have to specify sub 
    * object types separately. This can of course be done withing a 
    * single type declaration, but probably better to actually have
    * separate types for each sub property
    */
    type Whatever = Readonly<{
        k: string,
        m: string,
    }>
    type Something = Readonly<{
        a: number,
        b: string,
        else: readonly number[],
        whatever: Whatever,
    }>
    type Thing = Readonly<{
        id: number,
        name: string,
        attributes: readonly number[],
        something: Something
    }>

    const thing: Thing = {
        id: 5,
        name: 'uliui',
        attributes: [
            1, 2, 3
        ],
        something: {
            a: 5,
            b: 'jrytyjyjt',
            else: [
                5, 3, 1
            ],
            whatever: {
                k: 'wrth',
                m: 'kyruk',
            }
        }
    }

    // thing.name = 'rtetyj'; // error
    // thing.attributes[0] = 6; // error
    // thing.something.b = 'rtetyj'; // error
    // thing.something.else[1] = 6; // error
    // thing.something.whatever.m = 'wrth'; // error
}

export default () => {
    readonlyTuple();
    readonlyArray();
    readonlyProperty();
    readonlyUtilityType();
}
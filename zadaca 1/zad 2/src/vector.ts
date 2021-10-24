//Custom clasa za rad sa vektorima
class Vector {
    x: number;
    y: number;
    z: number;

    constructor(x: number = 0, y: number = 0, z: number = 0) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    copy = (): Vector => new Vector(this.x, this.y, this.z)

    normalize = () => {
        const n = this.norm();

        return new Vector(this.x / n, this.y / n, this.z / n);
    }

    norm = (): number => Math.sqrt(Math.pow(this.x, 2) + Math.pow(this.y, 2) + Math.pow(this.z, 2));

    prod = (v: Vector): number => this.x * v.x + this.y * v.y + this.z * v.z;

    scalProd = (n: number): Vector => new Vector(this.x * n, this.y * n, this.z * n);

    vecProd = (v: Vector): Vector => {
        let result = new Vector();

        result.x = this.y * v.z - this.z * v.x;
        result.y = -(this.x * v.z - this.z * v.x);
        result.z = this.x * v.y - this.y * v.x

        return result;
    }

    add = (v: Vector): Vector => {
        this.x += v.x;
        this.y += v.y;
        this.z += v.z;

        return this;
    }

    sub = (v: Vector): Vector => {
        this.x -= v.x;
        this.y -= v.y;
        this.z -= v.z;

        return this;
    }

    fromAngle = (angle: number): Vector => {
        const rad = angle * Math.PI / 180

        let t = new Vector(parseFloat(Math.cos(rad).toFixed(6)), parseFloat(Math.sin(rad).toFixed(6)));

        return this.copy().add(t);
    }

    static angle = (angle: number): Vector => {
        const rad = angle * Math.PI / 180

        return new Vector(parseFloat(Math.cos(rad).toFixed(6)), parseFloat(Math.sin(rad).toFixed(6)));
    }

    toString = (): string => `${this.x} ${this.y} ${this.z}`;
    toNormal = (): string => `vn ${this.x} ${this.y} ${this.z}`;
}

export default Vector;
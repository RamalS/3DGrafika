import fs from "fs"
import Vector from "./vector";

class Cilindre {
    resolution: number;
    radius: number;
    height: number;

    constructor(resolution: number, radius: number, height: number) {
        this.resolution = resolution;
        this.radius = radius;
        this.height = height;
    }

    private mainBase = (): Vector[] => {
        let result: Vector[] = [];

        const angle = 360 / this.resolution;
        let currAngle = angle;

        for (let i = 0; i < this.resolution; i++) {
            const v = Vector.angle(currAngle).scalProd(this.radius);
            result.push(v)
            currAngle += angle;
        }

        return result;
    }

    private secondBase = (vectors: Vector[]): Vector[] => {
        let result: Vector[] = []

        let a = vectors[0].normalize();
        let b = vectors[1].normalize();

        let vert = a.vecProd(b).normalize().scalProd(this.height);

        for (const i in vectors) {
            const vec = vectors[i];
            const t = vec.copy().add(vert);

            result.push(t)
        }

        return result;
    }

    private vector = (v: Vector): string => `v ${v} \n`;
    private normals = (n: Vector): string => `vn ${n} \n`
    private face = (v1: number, n1: number, v2: number, n2: number, v3: number, n3: number): string => `f ${v1}//${n1} ${v2}//${n2} ${v3}//${n3} \n`

    public save = (fileName: string): void => {
        const mainB = this.mainBase();
        const secondB = this.secondBase(mainB);

        const mLen = mainB.length;
        const sLen = secondB.length;

        let vectors = ""
        let normals = ""
        let faces = ""
        let nCount = 0;

        for (const i in mainB) vectors += this.vector(mainB[i]);
        for (const i in mainB) vectors += this.vector(secondB[i]);

        normals += this.normals(mainB[1].vecProd(mainB[0]).normalize());
        nCount++;

        for (let i = 1; i < mLen - 1; i++) faces += this.face(1, nCount, i + 1, nCount, i + 2, nCount);

        for (let i = 0; i < mLen; i++) {
            normals += this.normals(mainB[i].normalize());
            normals += this.normals(mainB[(i + 1) < mLen ? i + 1 : (i + 1) % mLen].normalize());
            nCount += 2;

            faces += this.face(i > 0 ? i + mLen : mLen + sLen, nCount - 4 > 1 ? nCount - 4 : mLen * 2, i + mLen + 1, nCount - 1, i + 1, nCount - 1);
            faces += this.face(i + 1, nCount - 1, i + mLen + 1, nCount - 1, (i + 2) < mLen + 1 ? (i + 2) : 1, nCount);
        }

        normals += this.normals(mainB[0].vecProd(mainB[1]).normalize());
        nCount++;

        for (let i = 1; i < sLen - 1; i++) faces += this.face(mLen + 1, nCount, i + mLen + 1, nCount, i + mLen + 2, nCount);


        const result = `#Base resolution ${this.resolution}\n\n${vectors}\n ${normals}\n ${faces}`;

        fs.writeFileSync(`${fileName}.obj`, result);
    }
}


const cilindre = new Cilindre(365, 1, 2);
cilindre.save("object");

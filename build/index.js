"use strict";

var Module = {
    postRun: () => {
        /**
         * @param {Float32Array} a 
         * @param {Float32Array} b 
         * @param {Float32Array} dst 
         */
        const MultiplyMatrixJS = function(a, b, dst) {
            for(let x = 0; x < 4; x++) {
                for(let y = 0; y < 4; y++) {
                    let i = x * 4;
                    dst[i + y] =
                        a[0 + y] * b[i + 0] 
                        + a[4 + y] * b[i + 1] 
                        + a[8 + y] * b[i + 2] 
                        + a[12 + y] * b[i + 3];
                }
            }
        };

        /**
         * @type {(m00:number, m10:number, m20:number, m30:number, m01:number, m11:number, m21:number, m31:number, m02:number, m12:number, m22:number, m32:number, m03:number, m13:number, m23:number, m33:number) => number}
         */
        const CreateMatrix = Module.cwrap('CreateMatrix', 'number', ['number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number']);

        /**
         * @type {(m:number) => void}
         */
        const TransposeMatrix = Module.cwrap('TransposeMatrix', '', ['number']);

        /**
         * @type {(a:number, b:number, dst:number) => void}
         */
        const MultiplyMatrix = Module.cwrap('MultiplyMatrix', '', ['number', 'number', 'number']);

        /**
         * @type {(m:number) => void}
         */
        const PrintMatrix = Module.cwrap('PrintMatrix', '', ['number']);

        const p1 = CreateMatrix(
            1, 5, 1, 5, 
            2, 6, 2, 6, 
            3, 7, 3, 7, 
            4, 8, 4, 8
        );

        const p2 = CreateMatrix(
            1, 5, 1, 5, 
            2, 6, 2, 6, 
            3, 7, 3, 7, 
            4, 8, 4, 8
        );

        const q = CreateMatrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );

        const _p1 = new Float32Array(Module.buffer, p1, 16);
        const _p2 = new Float32Array(Module.buffer, p2, 16);
        const _q = new Float32Array(Module.buffer, q, 16);

        let t1 = new Date();
        let n = 1000000;
        for(let i = 0; i < n; i++) {
            MultiplyMatrix(p1, p2, q);
        }
        let t2 = new Date();

        let el = document.createElement('p');
        el.textContent = `WASM: ${Math.floor(n / (t2 - t1))} ops / s`;
        document.body.appendChild(el);

        t1 = new Date();
        n = 1000000;
        for(let i = 0; i < n; i++) {
            MultiplyMatrixJS(_p1, _p2, _q);
        }
        t2 = new Date();

        el = document.createElement('p');
        el.textContent = `JS: ${Math.floor(n / (t2 - t1))} ops / s`;
        document.body.appendChild(el);

        PrintMatrix(q);
    }
};

import {IHarmonic} from "./IHarmonic.ts";

export class Harmonic implements IHarmonic {
    frequency: number;
    amplitude: number;
    phase: number;
    type: 'sin' | 'cos';

    constructor(frequency: number, amplitude: number, phase: number, type: 'sin' | 'cos') {
        this.frequency = frequency;
        this.amplitude = amplitude;
        this.phase = phase;
        this.type = type;
    }

    evaluate(x: number): number {
        const omega = 2 * Math.PI * this.frequency;
        const argument = omega * x + this.phase;

        return this.type === 'sin'
            ? this.amplitude * Math.sin(argument)
            : this.amplitude * Math.cos(argument);
    }
}

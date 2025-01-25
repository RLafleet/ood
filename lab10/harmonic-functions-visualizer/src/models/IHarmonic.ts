export interface IHarmonic {
    frequency: number;
    amplitude: number;
    phase: number;
    type: 'sin' | 'cos';

    evaluate(x: number): number;
}
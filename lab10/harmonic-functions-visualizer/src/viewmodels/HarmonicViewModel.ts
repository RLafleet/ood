import { IHarmonic } from '../models/IHarmonic.ts';
import { Harmonic} from "../models/Harmonic.ts";

export class HarmonicViewModel {
    private harmonics: IHarmonic[] = [];
    private listeners: (() => void)[] = [];

    addHarmonic(harmonic: IHarmonic): void {
        this.harmonics.push(new Harmonic(harmonic.frequency, harmonic.amplitude, harmonic.phase, harmonic.type));
        this.notify();
    }

    removeHarmonic(index: number): void {
        this.harmonics.splice(index, 1);
        this.notify();
    }

    updateHarmonic(index: number, updated: IHarmonic): void {
        this.harmonics[index] = new Harmonic(
            updated.frequency,
            updated.amplitude,
            updated.phase,
            updated.type
        );
        this.notify();
    }

    getHarmonics(): IHarmonic[] {
        return this.harmonics;
    }

    evaluate(x: number): number {
        return this.harmonics.reduce((sum, harmonic) => sum + harmonic.evaluate(x), 0);
    }

    subscribe(listener: () => void): void {
        this.listeners.push(listener);
    }

    private notify(): void {
        this.listeners.forEach((listener) => listener());
    }
}

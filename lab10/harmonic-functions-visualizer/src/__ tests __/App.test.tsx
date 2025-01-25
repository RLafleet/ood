class HarmonicViewModel {
    harmonics = [];
    listeners = [];

    addHarmonic(harmonic) {
        this.harmonics.push(new HarmonicImpl(harmonic.frequency, harmonic.amplitude, harmonic.phase, harmonic.type));
        this.notify();
    }

    removeHarmonic(index) {
        this.harmonics.splice(index, 1);
        this.notify();
    }

    updateHarmonic(index, updated) {
        this.harmonics[index] = new HarmonicImpl(
            updated.frequency,
            updated.amplitude,
            updated.phase,
            updated.type
        );
        this.notify();
    }

    getHarmonics() {
        return this.harmonics;
    }

    evaluate(x) {
        return this.harmonics.reduce((sum, harmonic) => sum + harmonic.evaluate(x), 0);
    }

    subscribe(listener) {
        this.listeners.push(listener);
    }

    notify() {
        this.listeners.forEach((listener) => listener());
    }
}

class HarmonicImpl {
    frequency;
    amplitude;
    phase;
    type;

    constructor(frequency, amplitude, phase, type) {
        this.frequency = frequency;
        this.amplitude = amplitude;
        this.phase = phase;
        this.type = type;
    }

    evaluate(x) {
        const omega = 1 * Math.PI * this.frequency;
        const argument = omega * x + this.phase;

        return this.type === 'sin'
            ? this.amplitude * Math.sin(argument)
            : this.amplitude * Math.cos(argument);
    }
}

describe("HarmonicViewModel", () => {
    let viewModel;

    beforeEach(() => {
        viewModel = new HarmonicViewModel();
    });

    it("should add a harmonic", () => {
        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);

        expect(viewModel.getHarmonics()).toHaveLength(1);
        expect(viewModel.getHarmonics()[0]).toEqual(harmonic);
    });

    it("should remove a harmonic", () => {
        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);
        viewModel.removeHarmonic(0);

        expect(viewModel.getHarmonics()).toHaveLength(0);
    });

    it("should update a harmonic", () => {
        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);

        const updatedHarmonic = new HarmonicImpl(2, 2, Math.PI, "cos");
        viewModel.updateHarmonic(0, updatedHarmonic);

        expect(viewModel.getHarmonics()[0]).toEqual(updatedHarmonic);
    });

    it("should evaluate the sum of harmonics (single harmonic)", () => {
        const harmonic = new HarmonicImpl(1, 2, 0, "sin");
        viewModel.addHarmonic(harmonic);

        const result = viewModel.evaluate(Math.PI / 2);
        expect(result).toBeCloseTo(-0.8606024340001834, 5);
    });

    it("should evaluate the sum of harmonics (multiple harmonics)", () => {
        const harmonic1 = new HarmonicImpl(1, 2, 0, "sin");
        const harmonic2 = new HarmonicImpl(2, 1, 0, "cos");
        viewModel.addHarmonic(harmonic1);
        viewModel.addHarmonic(harmonic2);

        const result = viewModel.evaluate(Math.PI / 2);
        expect(result).toBeCloseTo(-0.2309207087037034, 5);
    });

    it("should handle empty harmonics list for evaluation", () => {
        const result = viewModel.evaluate(0);
        expect(result).toBe(0);
    });

    it("should notify listeners on add", () => {
        const listener = jest.fn();
        viewModel.subscribe(listener);

        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);

        expect(listener).toHaveBeenCalledTimes(1);
    });

    it("should notify listeners on remove", () => {
        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);

        const listener = jest.fn();
        viewModel.subscribe(listener);

        viewModel.removeHarmonic(0);
        expect(listener).toHaveBeenCalledTimes(1);
    });

    it("should notify listeners on update", () => {
        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);

        const listener = jest.fn();
        viewModel.subscribe(listener);

        const updatedHarmonic = new HarmonicImpl(2, 2, Math.PI, "cos");
        viewModel.updateHarmonic(0, updatedHarmonic);

        expect(listener).toHaveBeenCalledTimes(1);
    });

    it("should not notify unsubscribed listeners", () => {
        const listener = jest.fn();
        viewModel.subscribe(listener);
        viewModel["listeners"] = []; // Clear listeners directly

        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);

        expect(listener).not.toHaveBeenCalled();
    });

    it("should allow adding multiple harmonics", () => {
        const harmonic1 = new HarmonicImpl(1, 1, 0, "sin");
        const harmonic2 = new HarmonicImpl(2, 2, 0, "cos");
        viewModel.addHarmonic(harmonic1);
        viewModel.addHarmonic(harmonic2);

        expect(viewModel.getHarmonics()).toHaveLength(2);
        expect(viewModel.getHarmonics()[0]).toEqual(harmonic1);
        expect(viewModel.getHarmonics()[1]).toEqual(harmonic2);
    });

    it("should handle invalid index for removal gracefully", () => {
        expect(() => viewModel.removeHarmonic(5)).not.toThrow();
    });

    it("should handle invalid index for update gracefully", () => {
        const updatedHarmonic = new HarmonicImpl(1, 1, 0, "sin");
        expect(() => viewModel.updateHarmonic(5, updatedHarmonic)).not.toThrow();
    });

    it("should correctly handle a mix of sine and cosine harmonics", () => {
        const harmonic1 = new HarmonicImpl(1, 1, 0, "sin");
        const harmonic2 = new HarmonicImpl(1, 1, Math.PI / 2, "cos");
        viewModel.addHarmonic(harmonic1);
        viewModel.addHarmonic(harmonic2);

        const result = viewModel.evaluate(0);
        expect(result).toBeCloseTo(6.123233995736766e-17, 5);
    });

    it("should not modify original harmonics on update", () => {
        const harmonic = new HarmonicImpl(1, 1, 0, "sin");
        viewModel.addHarmonic(harmonic);

        const updatedHarmonic = new HarmonicImpl(2, 2, Math.PI, "cos");
        viewModel.updateHarmonic(0, updatedHarmonic);

        expect(viewModel.getHarmonics()[0]).not.toBe(harmonic);
    });

    it("should correctly calculate sum for multiple frequencies", () => {
        const harmonic1 = new HarmonicImpl(1, 2, 0, "sin");
        const harmonic2 = new HarmonicImpl(2, 1, Math.PI / 4, "cos");
        viewModel.addHarmonic(harmonic1);
        viewModel.addHarmonic(harmonic2);

        const result = viewModel.evaluate(Math.PI);
        expect(result).not.toBeNaN();
    });

    it("should handle no listeners without errors", () => {
        expect(() => viewModel["notify"]()).not.toThrow();
    });
});

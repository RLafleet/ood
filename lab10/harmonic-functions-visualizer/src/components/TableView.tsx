import { useState } from "react";
import { HarmonicViewModel } from "../viewmodels/HarmonicViewModel";
import { IHarmonic } from "../models/IHarmonic.ts";
import { Harmonic} from "../models/Harmonic.ts";
import * as React from "react";

export const TableView: React.FC<{ viewModel: HarmonicViewModel }> = ({ viewModel }) => {
    const [harmonics, setHarmonics] = useState(viewModel.getHarmonics());

    const updateHarmonics = () => setHarmonics([...viewModel.getHarmonics()]);

    viewModel.subscribe(updateHarmonics);

    const handleUpdate = (index: number, field: keyof IHarmonic, value: string) => {
        const updated = { ...harmonics[index], [field]: field === 'type' ? value : parseFloat(value) } as IHarmonic;
        viewModel.updateHarmonic(index, updated);
    };

    const handleRemove = (index: number) => viewModel.removeHarmonic(index);

    const handleAdd = () => {
        const newHarmonic = new Harmonic(1, 1, 0, 'sin');
        viewModel.addHarmonic(newHarmonic);
    };

    return (
        <div>
            <table>
                <thead>
                <tr>
                    <th>Frequency</th>
                    <th>Amplitude</th>
                    <th>Phase</th>
                    <th>Type</th>
                    <th>Actions</th>
                </tr>
                </thead>
                <tbody>
                {harmonics.map((h, index) => (
                    <tr key={index}>
                        <td>
                            <input
                                type="number"
                                value={h.frequency}
                                onChange={(e) => handleUpdate(index, "frequency", e.target.value)}
                            />
                        </td>
                        <td>
                            <input
                                type="number"
                                value={h.amplitude}
                                onChange={(e) => handleUpdate(index, "amplitude", e.target.value)}
                            />
                        </td>
                        <td>
                            <input
                                type="number"
                                value={h.phase}
                                onChange={(e) => handleUpdate(index, "phase", e.target.value)}
                            />
                        </td>
                        <td>
                            <select
                                value={h.type}
                                onChange={(e) => handleUpdate(index, "type", e.target.value)}
                            >
                                <option value="sin">Sin</option>
                                <option value="cos">Cos</option>
                            </select>
                        </td>
                        <td>
                            <button onClick={() => handleRemove(index)}>Remove</button>
                        </td>
                    </tr>
                ))}
                </tbody>
            </table>
            <button onClick={handleAdd}>Add Harmonic</button>
        </div>
    );
};

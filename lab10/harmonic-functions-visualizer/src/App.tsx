import { TabSwitcher } from './components/TabSwitcher';
import { HarmonicViewModel } from './viewmodels/HarmonicViewModel';
import { Harmonic} from "./models/Harmonic.ts";
import * as React from "react";

const viewModel = new HarmonicViewModel();
viewModel.addHarmonic(new Harmonic(1, 1, 0, 'sin'));
viewModel.addHarmonic(new Harmonic(2, 0.5, Math.PI / 4, 'cos'));

const App: React.FC = () => {
    return (
        <div>
            <h1>Визуализатор гармонических функций</h1>
            <TabSwitcher viewModel={viewModel} />
        </div>
    );
};

export default App;

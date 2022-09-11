import React, { useEffect, useState } from 'react';
import './App.css';

const PATH = '/led';

function App() {
  const [ledOff, setLedOff] = useState<'0' | '1'>('0');
  const [isLoading, setIsLoading] = useState(true);

  const handleStateChange = () => {
    const value = ledOff === '0' ? '1' : '0';
    setIsLoading(true);
    fetch(PATH, { method: 'PUT', body: value })
      .then(response => response.text())
      .then((state) => setLedOff(state === '0' ? '1' : '0'))
      .then(() => setIsLoading(false));
  }

  useEffect(() => {
    fetch(PATH)
      .then(response => response.text())
      .then(state => setLedOff(state === '0' ? '1' : '0'))
      .then(() => setIsLoading(false));
  }, []);

  return (
    <header>
      <input
        type={'checkbox'}
        disabled={isLoading}
        checked={ledOff === '1'}
        onChange={handleStateChange}>
      </input>
    </header>
  );
}

export default App;

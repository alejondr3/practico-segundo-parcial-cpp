"""Compila y ejecuta las dos versiones de los siete ejercicios."""
from pathlib import Path
import os
import subprocess

raiz = Path(__file__).resolve().parent
destino = raiz / 'build'
destino.mkdir(exist_ok=True)
resultados = []
for fuente in sorted(raiz.glob('ejercicio_*/*.cpp')):
    nombre = fuente.parent.name + '_' + fuente.stem
    programa = destino / (nombre + ('.exe' if os.name == 'nt' else ''))
    subprocess.run(['g++', '-std=c++17', '-Wall', '-Wextra', '-Wpedantic',
                    '-Werror', '-g', str(fuente), '-o', str(programa)], check=True)
    prueba = subprocess.run([str(programa)], check=True, capture_output=True, text=True)
    resultados.append(f'{nombre}: OK\n{prueba.stdout}')
    print(resultados[-1], end='')
print(f'\n{len(resultados)}/14 programas compilados y probados correctamente.')

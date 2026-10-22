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
    entrada = None
    if fuente.parent.name == 'ejercicio_23':
        entrada = '8\n1 2 3 4 5 6 7 8\n1\n2\n7\n0\n'
    prueba = subprocess.run([str(programa)], input=entrada, check=True,
                            capture_output=True, text=True)
    if entrada is not None:
        if ('Lista actual: 1 7 3 4 5 6 2 8' not in prueba.stdout or
                'Recorrido inverso: 8 2 6 5 4 3 7 1' not in prueba.stdout):
            raise RuntimeError('El ejercicio 23 no produjo los recorridos esperados')
    resultados.append(f'{nombre}: OK\n{prueba.stdout}')
    print(resultados[-1], end='')
print(f'\n{len(resultados)}/14 programas compilados y probados correctamente.')

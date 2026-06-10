import os
import re

def clean_cpp(text):
    pattern = re.compile(r'//.*?$|/\*.*?\*/|"(?:\\.|[^\\"])*"|\'(?:\\.|[^\\\'])*\'', re.DOTALL | re.MULTILINE)
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'): return ''
        return s
    text = pattern.sub(replacer, text)
    return re.sub(r'\n\s*\n', '\n\n', text)

def clean_cmake(text):
    pattern = re.compile(r'#.*?$|"(?:\\.|[^\\"])*"', re.MULTILINE)
    def replacer(match):
        s = match.group(0)
        if s.startswith('#'): return ''
        return s
    text = pattern.sub(replacer, text)
    return re.sub(r'\n\s*\n', '\n\n', text)

for r, d, fs in os.walk('.'):
    if 'build' in r: continue
    for f in fs:
        path = os.path.join(r, f)
        if f.endswith(('.cpp', '.h')):
            with open(path, 'r', encoding='utf-8') as f_in:
                content = f_in.read()
            with open(path, 'w', encoding='utf-8') as f_out:
                f_out.write(clean_cpp(content))
        elif f == 'CMakeLists.txt':
            with open(path, 'r', encoding='utf-8') as f_in:
                content = f_in.read()
            with open(path, 'w', encoding='utf-8') as f_out:
                f_out.write(clean_cmake(content))

print("Comments removed successfully.")

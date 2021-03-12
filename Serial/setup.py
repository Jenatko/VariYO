from setuptools import setup

setup(name='serial_reader',
      version='0.1',
      description='Reading and plotting serial output from vario',
      url='https://github.com/Jenatko/VariYO',
      author='Juraj Palenik',
      author_email='juraj.palenik@protonmail.com',
      license='MIT',
      packages=['serial_reader'],
      install_requires=[
          'numpy',
          'matplotlib',
          'pyqt5',
          'pyserial'
      ],
      zip_safe=False)
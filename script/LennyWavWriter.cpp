#include "LennyWavWriter.h"

LennyWavWriter::LennyWavWriter(File f, int sampleRate) : _f(f)
{
  // Write out the header, fill in some of the blanks later
  _wavHeader.sample_rate = sampleRate;
  _f.write((uint8_t*)&_wavHeader, sizeof(wav_header_t));
  _fileSize = sizeof(wav_header_t);
}

void LennyWavWriter::write(int16_t *samples, int count)
{
  // Write the samples
  int bytesWritten = _f.write((uint8_t*)samples, sizeof(int16_t) * count);
  //Serial.println(bytesWritten);

  // Keep track of the file size so far
  _fileSize += sizeof(int16_t) * count;
}

int LennyWavWriter::end()
{
  // Fill in the header with the correct information and write it again
  _wavHeader.data_bytes = _fileSize - sizeof(wav_header_t);
  _wavHeader.wav_size = _fileSize - 8;
  _f.seek(0);
  _f.write((uint8_t*)&_wavHeader, sizeof(wav_header_t));

  return _fileSize;
}

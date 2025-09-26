# Qwen3OmniMoe Support in llama.cpp

This document describes the support for Qwen3OmniMoe models in llama.cpp.

## Overview

Qwen3OmniMoe is a multimodal mixture-of-experts model that can process text, images, video, and audio inputs. This implementation in llama.cpp focuses on the core "thinker" model which handles text generation with multimodal understanding.

## Architecture Components

The Qwen3OmniMoe model consists of several components:

1. **Thinker Model** (implemented in llama.cpp):
   - Text generation with MoE (Mixture of Experts) architecture
   - Vision encoder for image/video processing
   - Audio encoder for audio processing
   - Multimodal fusion capabilities

2. **Talker Model** (not included in llama.cpp):
   - Converts text to audio codec tokens
   - Required for audio generation

3. **Code2Wav Module** (not included in llama.cpp):
   - Converts audio codec tokens to waveforms
   - Required for final audio output

## Features Supported

- ✅ Text generation with multimodal context
- ✅ Image understanding (via vision encoder)
- ✅ Video understanding (via vision encoder)
- ✅ Audio understanding (via audio encoder)
- ✅ Mixture-of-Experts (MoE) inference
- ❌ Audio generation (requires talker and Code2Wav modules)

## Model Conversion

To convert a Qwen3OmniMoe model from Hugging Face format to GGUF:

```bash
python convert_hf_to_gguf.py /path/to/qwen3omnimoe/model --outtype f16
```

The conversion script will:
- Extract the thinker model weights
- Convert audio encoder tensors
- Handle MoE expert weights
- Skip talker and Code2Wav components (not needed for text generation)

## Usage

### Basic text generation:
```bash
./tools/main/main -m qwen3omnimoe.gguf -p "Describe this image: <image>" --image image.jpg
```

### With audio input (requires preprocessing):
```bash
./tools/main/main -m qwen3omnimoe.gguf -p "What do you hear in this audio? <audio>" --audio audio.wav
```

### Audio output placeholder:
```bash
./tools/main/main -m qwen3omnimoe.gguf -p "Say hello" --audio-output output.wav
```

Note: The `--audio-output` flag will save a placeholder WAV file. Full audio generation requires the complete Qwen3OmniMoe pipeline with talker and Code2Wav modules.

## Technical Details

### Audio Encoder

The audio encoder processes mel-spectrogram features through:
- Convolutional layers for downsampling
- Transformer encoder layers with self-attention
- Projection layers to match text embedding dimensions

### Supported Tensors

New tensor types added for audio encoder:
- `audio_encoder.pos_embd` - Positional embeddings
- `audio_encoder.blk.{n}.attn_norm` - Attention layer normalization
- `audio_encoder.blk.{n}.attn_{q,k,v}` - Attention projections
- `audio_encoder.blk.{n}.attn_output` - Attention output projection
- `audio_encoder.blk.{n}.ffn_norm` - FFN layer normalization
- `audio_encoder.blk.{n}.ffn_{gate,down}` - FFN layers

### Model Parameters

Audio encoder parameters stored in GGUF:
- `audio_encoder.encoder_layers` - Number of encoder layers
- `audio_encoder.d_model` - Model dimension
- `audio_encoder.encoder_attention_heads` - Number of attention heads
- `audio_encoder.encoder_ffn_dim` - FFN intermediate dimension
- `audio_encoder.num_mel_bins` - Number of mel frequency bins
- `audio_encoder.max_source_positions` - Maximum audio sequence length
- `audio_encoder.output_dim` - Output projection dimension

## Limitations

1. **Audio Generation**: Full audio synthesis requires the talker and Code2Wav components which are not included in llama.cpp. The model can understand audio inputs but cannot generate audio outputs beyond placeholder files.

2. **Audio Preprocessing**: Audio inputs need to be preprocessed into mel-spectrograms before being fed to the model. This preprocessing is not included in llama.cpp.

3. **Memory Usage**: The MoE architecture with audio/vision encoders requires significant memory. Use quantization for better memory efficiency.

## Future Work

- Implement audio preprocessing pipeline
- Add support for streaming audio input
- Optimize memory usage for multimodal inference
- Consider partial talker model integration for basic audio output

## References

- [Qwen2.5-Omni Model Repository](https://github.com/QwenLM/Qwen2.5-Omni)
- [llama.cpp Documentation](https://github.com/ggerganov/llama.cpp)

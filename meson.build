project('audio_programming', 'c')

cc = meson.get_compiler('c')

m = cc.find_library('m')
pulse = dependency('libpulse')
simple_pulse = dependency('libpulse-simple')
alsa = dependency('alsa')
glib2 = dependency('glib-2.0')

pcm16s = library('pcm16s', 'pcm16s.c', dependencies : [m])
stdio_playback = library('stdio_playback', 'stdio_playback.c', dependencies : pulse)

executable(
	'simple_pa_waveforms',
	'simple_pa/waveform_player.c',
	dependencies : [simple_pulse],
	link_with : pcm16s,
)

executable(
	'simple_pa_recorder',
	'simple_pa/simple_pa_recorder.c',
	dependencies : [simple_pulse],
)

executable(
	'pa_client',
	'pa_client.c',
	dependencies : pulse,
	link_with : stdio_playback,
)

executable(
	'alsalist',
	'alsa/list_devices.c',
	dependencies : alsa
)

executable(
	'rawmidi',
	'alsa/rawmidi_read.c',
	dependencies : alsa
)

executable(
	'rawmidi_write',
	'alsa/rawmidi_write.c',
	dependencies : alsa
)

executable(
	'pcm_from_file',
	'alsa/pcm_from_file.c',
	dependencies : [alsa, glib2],
)

executable(
	'print_midi_events',
	'alsa/print_midi_events.c',
	dependencies : alsa
)


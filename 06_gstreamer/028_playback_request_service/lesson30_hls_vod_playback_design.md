# 30강. HLS VOD Playback 전체 구조 설계

## 목표

녹화된 CCTV 영상을 사용자가 시간 범위로 요청하고, 서버가 HLS VOD session을 생성하여 브라우저/클라이언트에서 재생 및 seek를 지원하는 구조를 설계한다.

## 최종 구조

```txt
CameraPipeline
  ↓
splitmuxsink MP4 archive
  ↓
RecordingSegmentStore
  ↓
PlaybackService
  ↓
HlsVodSessionManager
  ↓
HlsVodConverter
  ↓
HlsVodPlaylistBuilder
  ↓
HttpPlaybackServer
  ↓
Browser / VMS Client
```

## 1차 구현 전략

- 기존 splitmuxsink 기반 MP4 archive 구조를 유지한다.
- 사용자의 playback request가 들어오면 필요한 MP4 segment를 조회한다.
- 조회된 segment를 playback cache 아래 HLS VOD session으로 변환한다.
- 생성된 `index.m3u8`와 `.ts` segment를 HTTP로 제공한다.
- 브라우저/HLS player는 m3u8를 기반으로 재생 및 seek를 수행한다.

## 주요 객체

### PlaybackRequest

```cpp
struct PlaybackRequest {
    std::string camera_id;
    TimePoint from;
    TimePoint to;
};
```

### PlaybackSession

```cpp
struct PlaybackSession {
    std::string session_id;
    std::string camera_id;
    TimePoint from;
    TimePoint to;
    std::vector<PlaybackSegment> segments;
    std::filesystem::path cache_dir;
    std::filesystem::path playlist_path;
};
```

### HlsVodSegment

```cpp
struct HlsVodSegment {
    int sequence_no = 0;
    std::string uri;
    std::filesystem::path file_path;
    int64_t duration_ms = 0;
    TimePoint program_date_time;
};
```

### HlsVodSession

```cpp
struct HlsVodSession {
    std::string session_id;
    std::string camera_id;
    TimePoint from;
    TimePoint to;
    std::filesystem::path cache_dir;
    std::filesystem::path playlist_path;
    std::vector<HlsVodSegment> hls_segments;
};
```

## Playback request flow

```txt
POST /api/playback/sessions
  ↓
PlaybackService::create_session(request)
  ↓
RecordingSegmentStore::make_playback_playlist(camera_id, from, to)
  ↓
HlsVodSessionManager::create_session(playlist)
  ↓
HlsVodConverter::convert_mp4_segments_to_hls(session)
  ↓
HlsVodPlaylistBuilder::write_index_m3u8(session)
  ↓
return playlistUrl
```

## HLS VOD output example

```m3u8
#EXTM3U
#EXT-X-VERSION:3
#EXT-X-PLAYLIST-TYPE:VOD
#EXT-X-TARGETDURATION:5
#EXT-X-MEDIA-SEQUENCE:0
#EXT-X-PROGRAM-DATE-TIME:2026-07-15T10:00:00.000+09:00
#EXTINF:5.000,
seg_000.ts
#EXT-X-PROGRAM-DATE-TIME:2026-07-15T10:00:05.000+09:00
#EXTINF:5.000,
seg_001.ts
#EXT-X-ENDLIST
```

## Seek model

브라우저/HLS player의 seek는 서버가 RTSP seek를 직접 처리하는 방식이 아니다. 서버는 시간 범위에 맞는 VOD playlist와 segment를 제공하고, 클라이언트가 필요한 segment를 HTTP로 요청한다.

CCTV 절대시간 변환:

```txt
absolute_time = playback_session.from + player.currentTime
```

또는 segment별 `EXT-X-PROGRAM-DATE-TIME`을 기준으로 실제 시간을 매핑한다.

## 이후 강의 흐름

```txt
31강. PlaybackRequest / PlaybackService 만들기
32강. Seek 요청을 segment offset으로 변환하기
33강. HLS VOD Session 개념 만들기
34강. HLS VOD playlist 생성기 만들기
35강. 단일 MP4 segment를 HLS TS segment로 변환하기
36강. 여러 MP4 segment를 HLS VOD session으로 변환하기
37강. 요청 구간 시작 offset 적용하기
38강. HTTP로 HLS VOD 제공하기
39강. 브라우저 HLS player에서 seek 확인
40강. CCTV 절대시간 기반 seek UI 설계
```

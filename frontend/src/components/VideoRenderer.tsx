import React from 'react';
import { Film, Play, Download } from 'lucide-react';

const VideoRenderer: React.FC = () => {
  return (
    <div className="flex flex-col h-full">
      <div className="h-12 border-b border-slate-700 flex items-center justify-between px-4 bg-slate-800/50">
        <h2 className="text-lg font-semibold text-slate-200">视频合成器</h2>
        <div className="flex items-center gap-2">
          <button className="flex items-center gap-2 px-4 py-1.5 bg-emerald-600 hover:bg-emerald-700 rounded-md text-sm font-medium transition-colors">
            <Play className="w-4 h-4" />
            开始渲染
          </button>
          <button className="flex items-center gap-2 px-4 py-1.5 bg-slate-600 hover:bg-slate-700 rounded-md text-sm font-medium transition-colors">
            <Download className="w-4 h-4" />
            导出
          </button>
        </div>
      </div>
      <div className="flex-1 p-6 flex items-center justify-center">
        <div className="text-center">
          <Film className="w-16 h-16 text-slate-600 mx-auto mb-4" />
          <h3 className="text-xl font-semibold text-slate-400 mb-2">视频合成面板</h3>
          <p className="text-slate-500">在这里可以将所有素材合成为最终的视频</p>
        </div>
      </div>
    </div>
  );
};

export default VideoRenderer;

import React from 'react';
import { Layout, Film, Camera } from 'lucide-react';

const StoryboardEditor: React.FC = () => {
  return (
    <div className="flex flex-col h-full">
      <div className="h-12 border-b border-slate-700 flex items-center justify-between px-4 bg-slate-800/50">
        <h2 className="text-lg font-semibold text-slate-200">分镜编辑器</h2>
        <button className="flex items-center gap-2 px-4 py-1.5 bg-purple-600 hover:bg-purple-700 rounded-md text-sm font-medium transition-colors">
          <Camera className="w-4 h-4" />
          自动分镜
        </button>
      </div>
      <div className="flex-1 p-6 flex items-center justify-center">
        <div className="text-center">
          <Layout className="w-16 h-16 text-slate-600 mx-auto mb-4" />
          <h3 className="text-xl font-semibold text-slate-400 mb-2">分镜设计面板</h3>
          <p className="text-slate-500">在这里可以设计和编辑故事的分镜头</p>
        </div>
      </div>
    </div>
  );
};

export default StoryboardEditor;
